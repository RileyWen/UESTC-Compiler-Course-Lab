//
// Created by rileywen on 9/24/18.
//

#ifndef TOYCOMPILER_TOKEN_H
#define TOYCOMPILER_TOKEN_H

#include <string>

namespace Tag {
    int NUM = 256, ID = 257, REAL = 258;

    //EQ, NE,
};

class Token {
public:
    int tag;

    explicit Token(int t) : tag(t) {}
};

class Num : public Token {
public:
    int value;

    explicit Num(int v) : Token(Tag::NUM), value(v) {}
};

class Real : public Token {
public:
    double value;

    explicit Real(double v) : Token(Tag::REAL), value(v) {}
};

class Word : public Token {
public:
    std::string lexeme;

    explicit Word(std::string &&s, int tag) : Token(tag), lexeme(s) {}
};

#endif //TOYCOMPILER_TOKEN_H
