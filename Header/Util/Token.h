//
// Created by rileywen on 9/24/18.
//

#ifndef TOYCOMPILER_TOKEN_H
#define TOYCOMPILER_TOKEN_H

#include <string>

enum class Tag : int {
    NUM, ID, REAL,

    ADD, SUB, MUL, DIV,

    EQ, NE,

};

class Token {
public:
    const Tag tag;

    explicit Token(Tag t) : tag(t) {}
};

class Num : Token {
public:
    const int value;

    explicit Num(int v) : Token(Tag::NUM), value(v) {}
};

class Real : Token {
public:
    const double value;

    explicit Real(double v) : Token(Tag::REAL), value(v) {}
};

#endif //TOYCOMPILER_TOKEN_H
