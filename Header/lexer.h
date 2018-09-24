//
// Created by rileywen on 9/24/18.
//

#ifndef TOYCOMPILER_LEXER_H
#define TOYCOMPILER_LEXER_H

#include <iostream>
#include <unordered_map>
#include "Util/token.h"

class Lexer {
public:
    static int line;

    Token *scan();

private:
    char peek = ' ';
    std::unordered_map<std::string, Word *> words;

    void reserve(Word *w) { words[w.lexeme] = w; }

    void readch() { std::cin.get(peek); }

    bool readch(char c);

    void retract() { std::cin.unget(); }

    int ctoi(char c) { return (int) (c - '0'); }

};

int Lexer::line = 1;

#endif //TOYCOMPILER_LEXER_H
