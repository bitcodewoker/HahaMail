//
// Created by ljc on 9/16/20.
// Copyright (c) 2020 ljc All rights reserved.
//

# include "server.h"


void Server :: save_draft(cJSON *content, int fd) {
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *sender = cJSON_Print(cJSON_GetObjectItem(content, "sender"));
    char *title = cJSON_Print(cJSON_GetObjectItem(content, "title"));
    char *body = cJSON_Print(cJSON_GetObjectItem(content, "body"));
    char *recipient = cJSON_Print(cJSON_GetObjectItem(content, "recipient"));
    Prec(sender);
    Prec(title);
    Prec(body);
    Prec(recipient);
    Email draft{};
    draft.sender = new char[30];
    draft.recipient = new char[30];
    draft.title = new char[100];
    draft.body = new char[10000];
    sprintf(draft.sender, sender);
    sprintf(draft.recipient, recipient);
    sprintf(draft.title, title);
    sprintf(draft.body, body);
    Status draft_status = mysql_dao->SaveDraft(DB, "null", sender, &draft);
    char buf[50];
    if (draft_status == EXPECTED_SUCCESS) {
        strcpy(buf, "EXPECTED_SUCCESS");
        printf("[console] Account %s save draft successfully\n");
    } else {
        strcpy(buf, "ERROR!");
        printf("[console] Account %s save draft unsuccessfully\n");
    }
    write(_connect_host[fd], buf, strlen(buf));
    delete mysql_dao;
    free(sender);
    free(recipient);
    free(title);
    free(body);
}


void Server :: fetch_draft(cJSON *content, int fd) {
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *username = cJSON_Print(cJSON_GetObjectItem(content, "username"));
    printf("username:%s\n", username);
    EmailFeedback *draft_feedback = mysql_dao->FetchDraft(DB, "null", username);
    printf("[console] email_status:%d, email_num:%d\n", draft_feedback->status, draft_feedback->email_num);


    cJSON *root = cJSON_CreateObject();
    cJSON *title = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "number", cJSON_CreateNumber(draft_feedback->email_num));
    cJSON_AddItemToObject(root, "content", title);
    if (draft_feedback->status == EXPECTED_SUCCESS)
        for (int i = 0; i < draft_feedback->email_num; i++) {
            char ss[50] = {0};
            sprintf( ss, "%d", i+1);
            cJSON_AddItemToObject(title, ss, cJSON_CreateString(draft_feedback->email[i]->title));
            cJSON_AddItemToObject(title, draft_feedback->email[i]->title,
                                  cJSON_CreateString(draft_feedback->email[i]->body));
            cJSON_AddItemToObject(title, draft_feedback->email[i]->title,
                                  cJSON_CreateString(draft_feedback->email[i]->sender));
            printf("[console] Email No.%d: sender: %s, recipient: %s, time: %s, title: %s, body: %s\n", i,
                   draft_feedback->email[i]->sender, draft_feedback->email[i]->recipient,
                   draft_feedback->email[i]->time, draft_feedback->email[i]->title, draft_feedback->email[i]->body);

        }

    // Close MySQL connection
    char *str = cJSON_PrintUnformatted(root);
    write(_connect_host[fd], str, strlen(str));
    delete draft_feedback;
    free(username);
}


void Server :: fetch_mail(cJSON *content, int fd) {
    printf("fetch!\n");
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *username = cJSON_Print(cJSON_GetObjectItem(content, "username"));
    printf("username:%s\n", username);
    char *type = cJSON_Print(cJSON_GetObjectItem(content, "type"));
    printf("type:%s\n", type);
    Prec(username);
    Prec(type);
    EmailType tp;
    if (strcmp(type, "\"inbox\"") == 0) {
        tp = IN;
    } else if (strcmp(type, "\"outbox\"") == 0) {
        tp = OUT;
    }
    EmailFeedback *email_feedback = mysql_dao->FetchEmail(DB, "null", username, tp);
    printf("[console] email_status:%d, email_num:%d\n", email_feedback->status, email_feedback->email_num);

    cJSON *root = cJSON_CreateObject();
    cJSON *title = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "number", cJSON_CreateNumber(email_feedback->email_num));
    cJSON_AddItemToObject(root, "content", title);
    if (email_feedback->status == EXPECTED_SUCCESS)
        for (int i = 0; i < email_feedback->email_num; i++) {
            char ss[50] = "title_";
            ss[6] = i + 1 + '0';
            ss[7] = '\0';
            cJSON_AddItemToObject(title, ss, cJSON_CreateString(email_feedback->email[i]->title));
            cJSON_AddItemToObject(title, email_feedback->email[i]->title,
                                  cJSON_CreateString(email_feedback->email[i]->body));
            if (tp == IN)
                cJSON_AddItemToObject(title, email_feedback->email[i]->title,
                                      cJSON_CreateString(email_feedback->email[i]->sender));
            else if (tp == OUT)
                cJSON_AddItemToObject(title, email_feedback->email[i]->title,
                                      cJSON_CreateString(email_feedback->email[i]->recipient));
            printf("[console] Email No.%d: sender: %s, recipient: %s, time: %s, title: %s, body: %s\n", i,
                   email_feedback->email[i]->sender, email_feedback->email[i]->recipient,
                   email_feedback->email[i]->time, email_feedback->email[i]->title, email_feedback->email[i]->body);

        }

    // Close MySQL connection
    char *str = cJSON_PrintUnformatted(root);
    write(_connect_host[fd], str, strlen(str));

    delete mysql_dao;
    free(username);
    free(type);
}
