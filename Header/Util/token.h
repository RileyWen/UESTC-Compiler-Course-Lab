//
// Created by rileywen on 9/24/18.
//
#include <string>

#ifndef TOYCOMPILER_TOKEN_H
#define TOYCOMPILER_TOKEN_H 1

namespace Tag {
    const static int NUM = 256, ID = 257, REAL = 258;

    const static int EQ = 259, LE = 260, GE = 261, UNKNOWN = 262;

    const static int STR = 263, NE = 264;
};

class Token {
public:
    int tag;

    explicit Token(int t) : tag(t) {}

    virtual ~Token() = default;
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

    explicit Word(std::string &s, int tag) : Token(tag), lexeme(s) {}
};

#endif //TOYCOMPILER_TOKEN_H
