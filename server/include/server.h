//
// Created by ljc on 9/15/20.
// Copyright (c) 2020 ljc All rights reserved.
//

#ifndef LINUX_EMAIL_SERVER_SERVER_H
#define LINUX_EMAIL_SERVER_SERVER_H

#include "mesdb.h"
#include "user.h"
#include "question.h"

#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<cjson/cJSON.h>
#include<pthread.h>

#define IP "127.0.0.1"
#define DB "192.168.43.207"
#define SERVER_PORT 7100//设置端口地址
#define BUFF_SIZE 1024
#define BACKLOG 5//设置监听队列长度
#define CLIENTUNM 1024//客户端最大数量



class Server {
private:
    int _server_fd;
    struct sockaddr_in _local;
    char _ip_s[50];
    int _connection;
    int _connect_host[CLIENTUNM];//可连接客户端文件描述数组
    int _connect_number;//已连接客户端数目
    pthread_t _thread_do[2];//线程ID
public:

    Server ();

    void Run ();

    [[noreturn]] void *handle_connect (void *argv);
    [[noreturn]] void *handle_request (void *argv);


    void registerr(cJSON *, int);
    void Prec (char *);

    void  ReadJson (cJSON *, int);
    void login(cJSON *, int);
    void fetch_mail(cJSON *, int);
    void recovery(cJSON *, int);
    void get_recovery_question(cJSON *content, int);
    void deletee(cJSON *content, int);
    void save_draft(cJSON *content, int);
    void fetch_draft(cJSON *content, int);
    void set_contact(cJSON *content, int);
    void get_contact(cJSON *content, int);
    void send_attachment(cJSON *content, int);
    void fetch_attachment(cJSON *content, int);
};


#endif //LINUX_EMAIL_SERVER_SERVER_H
