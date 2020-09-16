//
// Created by ljc on 9/16/20.
// Copyright (c) 2020 ljc All rights reserved.
//

# include "server.h"


void Server :: set_contact(cJSON *content, int fd) {
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *username = cJSON_Print(cJSON_GetObjectItem(content, "username"));
    printf("username:%s\n", username);
    char *fri = cJSON_Print(cJSON_GetObjectItem(content, "friend"));
    printf("friend:%s\n", fri);
    char *remark = cJSON_Print(cJSON_GetObjectItem(content, "remark"));
    printf("remark:%s\n", remark);
    Prec(username);
    Prec(fri);
    Prec(remark);
    Contact *contact = new Contact;
    contact->contact_name = new char[50];
    contact->alias = new char[50];
    strcpy(contact->contact_name, fri);
    strcpy(contact->alias, remark);
    Status contact_status = mysql_dao->SetContact(DB, "null", username, contact);
    printf("333\n");
    char buf[50];
    if (contact_status == EXPECTED_SUCCESS) {
        strcpy(buf, "EXPECTED_SUCCESS");

    } else {
        strcpy(buf, "ERROR!");
        printf("[console] Account %s fetch draft unsuccessfully\n");
    }
    write(_connect_host[fd], buf, strlen(buf));

    delete mysql_dao;
    free(username);
    free(fri);
    free(remark);
}

void Server :: get_contact(cJSON *content, int fd) {
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *username = cJSON_Print(cJSON_GetObjectItem(content, "username"));
    printf("username:%s\n", username);
    Prec(username);
    ContactFeedback *contact_feedback = mysql_dao->GetContact(DB, "null", username);
    cJSON *root = cJSON_CreateObject();
    cJSON *title = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "number", cJSON_CreateNumber(contact_feedback->contact_num));
    cJSON_AddItemToObject(root, "content", title);
    char str[10000];
    if (contact_feedback->status == EXPECTED_SUCCESS) {
        printf("%d\n", contact_feedback->contact_num);
        for (int i = 0; i < contact_feedback->contact_num; i++) {
            char ss[50] = "friend_";
            ss[6] = i + 1 + '0';
            ss[7] = '\0';
            cJSON_AddItemToObject(title, ss, cJSON_CreateString(contact_feedback->contact[i]->contact_name));
            cJSON_AddItemToObject(title, ss, cJSON_CreateString(contact_feedback->contact[i]->alias));
            strcpy(str, cJSON_PrintUnformatted(root));
            printf("%s %s\n", contact_feedback->contact[i]->contact_name, contact_feedback->contact[i]->alias);
        }
    } else {
        strcpy(str, "ERROR!");
    }
    write(_connect_host[fd], str, strlen(str));
    delete mysql_dao;
    free(username);
}


