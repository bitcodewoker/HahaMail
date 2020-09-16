//
// Created by ljc on 9/16/20.
// Copyright (c) 2020 ljc All rights reserved.
//

# include "question.h"

Question :: Question() {}

Question :: Question(string qus, string ans)
    : _question(qus), _answer(ans)
{
}

string Question :: GetQuestion() {
    return _question;
}

string Question :: GetAnswer() {
    return _answer;
}

bool Question :: CheckAnswer(string ans) {
    if (ans == _answer)
        return 1;
    return 0;
}

void Question :: Print() {
    std::cout << _question << std::endl << _answer << std::endl;
}

