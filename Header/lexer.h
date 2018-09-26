//
// Created by rileywen on 9/24/18.
//

#include <iostream>
#include <unordered_map>
#include <exception>
#include "Util/token.h"

#ifndef TOYCOMPILER_LEXER_H
#define TOYCOMPILER_LEXER_H 1

class Lexer {
public:
    static int line;

    Token *scan();

private:
    char peek = ' ';
    std::unordered_map<std::string, Word *> words;

    void reserve(Word *w) { words[w->lexeme] = w; }

    void readch() {
        std::cin.get(peek);
        if (peek == '\n') line++;
    }

    char nextch() { return (char) std::cin.peek(); }

    bool nextch(char c) { return (std::cin.peek() == c); }

    void retract() { std::cin.unget(); }

    int ctoi(char c) { return (int) (c - '0'); }

};

struct LexerException : public std::exception {
private:
    std::string exp_msg;
public:
    explicit LexerException(std::string &&exp_msg) : exp_msg(exp_msg) {}

    const char *what() const noexcept {
        return exp_msg.c_str();
    }
};


#endif //TOYCOMPILER_LEXER_H
