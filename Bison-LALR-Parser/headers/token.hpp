//
// Created by rileywen on 9/24/18.
//

#ifndef TOYCOMPILER_TOKEN_H
#define TOYCOMPILER_TOKEN_H 1

#include <string>
#include <memory>
#include <iostream>

using std::cout;

template<typename T>
using ptr = std::shared_ptr<T>;

template<typename T, typename... Args>
auto new_ptr(Args &&... args) -> decltype(std::make_shared<T>(args...)) {
    return std::make_shared<T>(args...);
}

template<typename T, typename... Args>
auto ptr_to(Args &&... args) -> decltype(std::dynamic_pointer_cast<T>(args...)) {
    return std::dynamic_pointer_cast<T>(args...);
}

namespace Tag {
    const static int NUM = 256, ID = 257, REAL = 258;

    const static int CMP = 259, ASSIGN = 260, TYPE = 261, UNKNOWN = 262;

    const static int STRING = 263, KEYWORD = 264, VOID = 265;

    const static int INDEX = 265, DESC = 266, EOF_ = 267, OP = 268;
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

    virtual ~Word() = default;
};

using Type=Word;

#endif //TOYCOMPILER_TOKEN_H
