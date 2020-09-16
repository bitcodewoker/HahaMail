//
// Created by ljc on 9/16/20.
// Copyright (c) 2020 ljc All rights reserved.
//

#ifndef LINUX_EMAIL_SERVER_USER_H
#define LINUX_EMAIL_SERVER_USER_H

# include "question.h"

# include <iostream>
# include <string>
using namespace std;

class User {
private:
    string _host;
    string _domain;
    string _password;
    string _nickname;
    string _description;
    Question _question;

public:
    User ();
    User (string, string, string, string, string, Question);


    string GetHost ();
    string GetDomain ();
    string GetNickName ();
    string GetDescription ();
    Question GetQuestion ();
    bool CheckPassword (string);
    void Print ();

};

#endif //LINUX_EMAIL_SERVER_USER_H
