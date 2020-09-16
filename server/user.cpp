//
// Created by ljc on 9/16/20.
// Copyright (c) 2020 ljc All rights reserved.
//

# include "user.h"

#include <utility>

User :: User () {}

User :: User(string host, string domain, string passs, string nick, string des, Question qus)
    : _host(std::move(host)), _domain(domain), _password(passs), _nickname(nick), _description(des), _question(qus)
{}

string User :: GetHost() {
    return _host;
}

string User :: GetDomain() {
    return _domain;
}

string User :: GetDescription() {
    return _description;
}

string User :: GetNickName() {
    return _nickname;
}

Question User :: GetQuestion() {
    return _question;
}

bool User :: CheckPassword(string pass) {
    if (pass == _password)
        return 1;
    return 0;
}

void User :: Print() {
    cout << _host << endl << _domain << endl << _password << endl << _description << _nickname << endl;
    _question.Print();
}