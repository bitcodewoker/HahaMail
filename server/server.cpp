//
// Created by ljc on 9/15/20.
// Copyright (c) 2020 ljc All rights reserved.
//

# include "server.h"

Server :: Server() {}

void * pre_request(void *argv) {
    auto *pre_c = static_cast<Server *>(argv);
    pre_c -> handle_request(argv);
    return nullptr;
}

void * pre_connect(void *argv) {
    auto *pre_c = static_cast<Server *>(argv);
    pre_c -> handle_connect(argv);
    return nullptr;
}

void Server :: Run() {
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&_local,0,sizeof(_local));
    _local.sin_family=AF_INET;
    _local.sin_addr.s_addr = htonl(INADDR_ANY);
//    _local.sin_addr.s_addr = inet_addr("127.0.0.1");
    _local.sin_port= htons(SERVER_PORT);



    if((bind(_server_fd,(struct sockaddr*)&_local,sizeof(_local)))==-1) {
        perror("fail to bind");
        exit(1);
    }

    listen(_server_fd,BACKLOG);

    pthread_create(&_thread_do[0],NULL, pre_connect,(void*)&_server_fd);//创建线程处理处理客户端连接

    pthread_create(&_thread_do[1],NULL, pre_request,NULL);//创建线程处理客户端请求

    for(int i=0;i<2;i++) {
        pthread_join(_thread_do[i],NULL);
        close(_server_fd);
    }
}

[[noreturn]] void *Server::handle_request(void *argv) {
    time_t now;
    char buff[BUFF_SIZE];

    int maxfd=-1;//最大监听文件描述符
    fd_set scanfd;//监听描述符集合

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec;

    for (;;) {
        maxfd = -1;
        FD_ZERO(&scanfd);

        for (int i = 0; i < CLIENTUNM; i++) {
            if (_connect_host[i] != -1) {
                FD_SET(_connect_host[i], &scanfd);
                if (maxfd < _connect_host[i]) {
                    maxfd = _connect_host[i];
                }
            }
        }

        int err = -1;
        err = select(maxfd + 1, &scanfd, NULL, NULL, &timeout);

        switch (err) {
            case 0 :
                break;
            case -1:
                break;
            default:
                if (_connect_number <= 0)
                    break;
                for (int i = 0; i < CLIENTUNM; i++) {
                    if(_connect_host[i]!=-1)//查找激活的文件描述符
                        if(FD_ISSET(_connect_host[i],&scanfd))//判断是否为合法文件描述符
                        {

                            memset(buff, 0, sizeof(buff));
                            int read_status = read(_connect_host[i], buff, sizeof (buff));
                            fputs(buff, stdout);
                            //write(_connect_host[i], buff, strlen(buff));

                            cJSON *root;
                            root = cJSON_Parse(buff);
                            printf("%s\n", cJSON_Print(root));

                            ReadJson (root, i);

                            strcpy(_ip_s, inet_ntoa(_local.sin_addr));
                            if (read_status == 0) {
                                exit(EXIT_SUCCESS);
                            } else if (read_status == -1) {
                                exit(EXIT_FAILURE);
                            }

                            _connect_host[i]=-1;//更新文件描述符的值
                            _connect_number --;//客户端计数器减1
                            close(_connect_host[1]);//关闭客户端
                        }
                }

        }
    }

}

[[noreturn]] void *Server::handle_connect(void *argv) {
    int sockfd=*((int *)argv);//获取服务器监听套接字文件描述符
    struct sockaddr_in from;
    socklen_t len=sizeof(from);

    /*接收客户端连接*/
    for(;;) {
        int i=0;
        int connfd=accept(sockfd,(struct sockaddr*)&from,&len);
        /*接收客户端的请求*/
        printf("a client connect,form:%s\n",inet_ntoa(from.sin_addr));//查找合适的位置，将客户端的文件描述符放入

        for(i=0;i<CLIENTUNM;i++) {
            if(_connect_host[i]==-1) {
                _connect_host[i]=connfd;//放入
                _connect_number ++;//客户端加1
                /*继续轮询等待客户端连接*/
                break;
            }
        }
    }
    return NULL;
}

void Server :: ReadJson(cJSON *data, int fd) {
    if (data == NULL) {
        printf ("error1:%s\n", cJSON_GetErrorPtr());
        cJSON_Delete(data);
        return;
    }
    cJSON * Command = cJSON_GetObjectItem(data, "command");
    if (Command == NULL) {
        printf("error:Command\n");
        cJSON_Delete(Command);
        return;
    }
    char *cmd = cJSON_Print(Command);
    printf("cmd:%s\n", cmd);
    cJSON *contentt = cJSON_GetObjectItem(data, "content");
    if (contentt == NULL) {
        printf("error:content\n");
        cJSON_Delete(contentt);
        return;
    }
    if (!strcmp(cmd, "\"send_mail\"")) {
        //send_mail( contentt );
    }
    else if (!strcmp(cmd, "\"get_recovery_question\"")) {
        get_recovery_question(contentt, fd);
    }
    else if (!strcmp(cmd, "\"delete\"")) {
        deletee(contentt, fd);
    }
    else if (!strcmp(cmd, "draft box")) {
        //draft( contentt );
    }
    else if (!strcmp(cmd, "friend")) {
        //friendd( contentt );
    }
    else if (!strcmp(cmd, "\"save_draft\"")) {
        save_draft(contentt, fd);
    }
    else if (!strcmp(cmd, "\"fetch_draft\"")) {
        fetch_draft(contentt, fd);
    }
    else if (!strcmp(cmd, "\"set_contact\"")) {
        set_contact(contentt, fd);
    }
    else if (!strcmp(cmd, "\"get_contact\"")) {
        get_contact(contentt, fd);
    }
    else if (!strcmp(cmd, "\"send_attachment\"")) {
        //send_attachment(contentt, fd);
    }
    else if (!strcmp(cmd, "\"fetch_attachment\"")) {
        //fetch_attachment(contentt, fd);
    }
    else if (!strcmp(cmd, "\"fetch_mail\"")) {
        fetch_mail(contentt, fd);
    }
    else if (!strcmp(cmd, "\"register\"")) {
        registerr (contentt, fd);
    }
    else if (!strcmp(cmd, "\"login\"")) {
        login(contentt, fd);
    }
    else if (!strcmp(cmd, "\"recovery\"")) {
        recovery(contentt, fd);
    }
    free(cmd);
}

//
//void Server :: send_attachment(cJSON *content, int fd) {
//    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
//    mysql_dao->Connect();
//    char *sender = cJSON_Print(cJSON_GetObjectItem(content, "sender"));
//    printf("sender:%s\n", sender);
//    char *title = cJSON_Print(cJSON_GetObjectItem(content, "title"));
//    printf("title:%s\n", title);
//    char *recipient = cJSON_Print(cJSON_GetObjectItem(content, "recipient"));
//    printf("recipient:%s\n", recipient);
//    cJSON* num = cJSON_GetObjectItem(content, "byte");
//    int byte = num->valueint;
//    printf( "the size of file is %d\n", byte );
//    Prec(title);
//    Prec(sender);
//    Prec(recipient);
//    char s[50] = "send_me_file";
//    write(_connect_host[fd], s, strlen(s) );
//    char buffer[BUFF_SIZE];
//    char file_name[100]="/home/zpy/CLionProjects/leaserver/";
//    strcat( file_name, title );
//    FILE *fp = fopen(file_name, "w");
//    if (fp == NULL){
//        printf("File:\t%s ERROR!\n", buffer);
//        char ss[50];
//        sprintf(ss, "File:\t%s ERROR!\n", buffer);
//        write(_connect_host[fd], ss, strlen(ss) );
//    }else{
//        bzero(buffer, BUFF_SIZE);
//        int file_block_length = 0;
//        while(1){
//            int read_status = read(_connect_host[fd], buffer, sizeof(buffer));
//            //fprintf(fp, buffer,)
//            if( read_status == -1 ){
//                char ss[50] = "read_error!";
//                printf( "read_error!" );
//                write(connection, ss, strlen(ss) );
//            }
//            if( read_status == 0 ){
//                char ss[50] = "read_success!";
//                printf( "read_success!" );
//                write(connection, ss, strlen(ss) );
//                break;
//            }
//            file_block_length += read_status;
//            if( read_status >= byte ){
//                break;
//            }
//            bzero(buffer, BUFF_SIZE);
//        }
//
//    }
//}


//void fetch_attachment(cJSON *content) {
//    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
//    mysql_dao->Connect();
//};


void Server :: Prec (char * str) {
    int n = strlen(str);
    for (int i = 1; i < n; i++) {
        if (str[i] == '\"') {
            str[i - 1] = '\0';
            return;
        }
        str[i - 1] = str[i];
    }
}



