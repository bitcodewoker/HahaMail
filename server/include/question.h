//
// Created by ljc on 9/16/20.
// Copyright (c) 2020 ljc All rights reserved.
//

#ifndef LINUX_EMAIL_SERVER_QUESTION_H
#define LINUX_EMAIL_SERVER_QUESTION_H

# include <string>
# include <iostream>
using namespace std;

class Question {
private:
    string _question;
    string _answer;

public:
    Question ();
    Question (string, string);

    string GetQuestion ();
    string GetAnswer ();
    bool CheckAnswer (string);
    void Print ();
};

#endif //LINUX_EMAIL_SERVER_QUESTION_H
