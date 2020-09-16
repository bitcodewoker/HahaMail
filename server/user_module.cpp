//
// Created by ljc on 9/16/20.
// Copyright (c) 2020 ljc All rights reserved.
//

# include "server.h"


void Server :: login(cJSON *content, int fd) {
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *username = cJSON_Print(cJSON_GetObjectItem(content, "username"));
    printf("username:%s\n", username);
    char *password = cJSON_Print(cJSON_GetObjectItem(content, "password"));
    printf("password:%s\n", password);
    Prec(username);
    Prec(password);

    SignInFeedback *feedback;
    feedback = mysql_dao->SignIn(DB, username, password);
    char buf[50];
    if (feedback->status == SignInStatus::SIGN_IN_SUCCESS) {
        strcpy(buf, "SIGN_IN_SUCCESS");
        printf("[console] Sign in successfully: token: %s, nickname: %s, description: %s\n", feedback->token,
               feedback->nickname, feedback->description);
    } else if (feedback->status == SignInStatus::SIGN_IN_ERROR) {
        strcpy(buf, "SIGN_IN_ERROR");
        printf("[console] Sign in error");
    } else if (feedback->status == SignInStatus::SIGN_IN_INVALID_ACCOUNT) {
        strcpy(buf, "SIGN_IN_INVALID_ACCOUNT");
        printf("[console] Account invalid");
    } else {
        strcpy(buf, "SIGN_IN_INVALID_PASSWD");
        printf("[console] Password invalid");
    }
    write(_connect_host[fd], buf, strlen(buf));
    delete (mysql_dao);
    free(username);
    free(password);
}


void Server :: registerr(cJSON *content, int fd) {
    // Init MySQL connection
    auto *mySqlDao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    // Connect to MySQL server
    mySqlDao->Connect();
    // Sign in function
    User user;
    char *host = cJSON_Print(cJSON_GetObjectItem(content, "host"));
    char *domin = cJSON_Print(cJSON_GetObjectItem(content, "domin"));
    char *password = cJSON_Print(cJSON_GetObjectItem(content, "password"));
    char *nickname = cJSON_Print(cJSON_GetObjectItem(content, "nickname"));
    char *description = cJSON_Print(cJSON_GetObjectItem(content, "description"));
    char *recovery_question = cJSON_Print(cJSON_GetObjectItem(content, "recovery_question"));
    char *recovery_answer = cJSON_Print(cJSON_GetObjectItem(content, "recovery_answer"));
    Prec(host);
    Prec(domin);
    Prec(password);
    Prec(nickname);
    Prec(description);
    Prec(recovery_question);
    Prec(recovery_answer);

    SignUpStatus signup_status;
    signup_status = mySqlDao->SignUp(_ip_s, host, domin, password, nickname, description, recovery_question,
                                     recovery_answer);
    char buf[50];
    if (signup_status == SignUpStatus::SIGN_UP_SUCCESS) {
        strcpy(buf, "SIGN_UP_SUCCESS");
        printf("[console] Account %s@%s sign up successfully\n", host, domin);
    } else if (signup_status == SignUpStatus::SIGN_UP_ACCOUNT_CONFLICT) {
        strcpy(buf, "SIGN_UP_ACCOUNT_CONFLICT");
        printf("[console] Account %s@%s sign up unsuccessfully, already exists\n", host, domin);
    } else {
        strcpy(buf, "UNEXPECTED_ERROR");
        printf("[console] Account %s@%s sign up unsuccessfully, unexpected error\n", host, domin);
    }
    write(_connect_host[fd], buf, strlen(buf));
    delete (mySqlDao);
    free(host);
    free(domin);
    free(password);
    free(nickname);
    free(description);
    free(recovery_question);
    free(recovery_answer);
    printf("1ok\n");
}


void Server :: recovery(cJSON *content, int fd) {
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *username = cJSON_Print(cJSON_GetObjectItem(content, "username"));
    printf("username:%s\n", username);
    char *recovery_answer = cJSON_Print(cJSON_GetObjectItem(content, "recovery_answer"));
    printf("recovery_answer:%s\n", recovery_answer);
    char *new_password = cJSON_Print(cJSON_GetObjectItem(content, "new_password"));
    printf("new_password:%s\n", new_password);
    Prec(username);
    Prec(recovery_answer);
    Prec(new_password);
    char buf[50];
    RecoverStatus recover_status = mysql_dao->Recover(DB, "null", username, recovery_answer, new_password);
    if (recover_status == RECOVER_SUCCESS) {
        strcpy(buf, "RECOVER_SUCCESS");
        printf("[console] Account %s recovers successfully\n", username);
    } else if (recover_status == RECOVER_INVALID_ANSWER) {
        strcpy(buf, "RECOVER_INVALID_ANSWER");
        printf("[console] Account %s recovers unsuccessfully: invalid answer\n", username);
    } else {
        strcpy(buf, "ERROR!");
        printf("[console] Account %s recovers unsuccessfully\n", username);
    }
    delete mysql_dao;
    free(username);
    free(recovery_answer);
    free(new_password);
}


void Server :: get_recovery_question(cJSON *content, int fd) {
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *username = cJSON_Print(cJSON_GetObjectItem(content, "username"));
    printf("username:%s\n", username);
    Prec(username);
    RecoverFeedback *recover_feedback = mysql_dao->GetRecoverQuestion(DB, username);
    char buf[500];
    if (recover_feedback->status == Status::EXPECTED_SUCCESS) {
        strcpy(buf, recover_feedback->question);
        printf("[console] Recovery question: %s\n", recover_feedback->question);
    } else {
        strcpy(buf, "ERROR!");
        printf("[console] Get recovery question unsuccessfully\n");
    }
    write(_connect_host[fd], buf, strlen(buf));
    delete recover_feedback;
    free(username);
}

void Server :: deletee(cJSON *content, int fd) {
    auto *mysql_dao = new MySQL_DAO(DB, 3306, "email_admin", "email_admin_passwd", "email_system");
    mysql_dao->Connect();
    char *username = cJSON_Print(cJSON_GetObjectItem(content, "username"));
    printf("username:%s\n", username);
    Prec(username);
    Status delete_status = mysql_dao->Delete(DB, "null", username);
    char buf[50];
    if (delete_status == EXPECTED_SUCCESS) {
        strcpy(buf, "EXPECTED_SUCCESS");
        printf("[console] Account %s deletes itself successfully\n", username);
    } else {
        strcpy(buf, "ERROR!");
        printf("[console] Account %s deletes itself unsuccessfully\n", username);
    }
    write(_connect_host[fd], buf, strlen(buf));
    delete mysql_dao;
    free(username);

}
