//
// Created by rileywen on 9/24/18.
//
#include "lexer.h"
#include <cctype>

bool Lexer::readch(char c) {
    readch();
    if (peek != c) return false;
    peek = ' ';
    return true;
}

Token *Lexer::scan() {
    for (;; readch()) {
        if (peek == ' ' || peek == '\t') continue;
        else if (peek == '\n') line++;
        else break;
    }
    switch (peek) {
        case:
            '/'
        case '=':
            if (readch('=')) return static_cast<Token *>(new Word(std::string("=="), Tag::EQ));
            else return new Token('=');
        case '<':
            if (readch('=')) return static_cast<Token *>(new Word(std::string("=="), Tag::LE));
            else return new Token('<');
        case '>':
            if (readch('=')) return static_cast<Token *>(new Word(std::string("=="), Tag::GE));
            else return new Token('>');
        default:
            break;
    }
    if (isdigit(peek)) {
        int v = 0;
        do {
            v = 10 * v + ctoi(peek);
            readch();
        } while (isdigit(peek));
        if (peek != '.')
            return static_cast<Token *>(new Num(v));

        double x = v, d = 10;
        for (;;) {
            readch();
            if (!isdigit(peek)) break;
            x += (double) ctoi(peek) / d;
            d *= 10;
        }
        return static_cast<Token *>(new Real(x));
    }

    if (isalpha(peek)) {
        std::string b;
        do {
            b += peek;
            readch();
        } while (isalpha(peek) || isdigit(peek));

        auto got = words.find(b);
        if (got != words.end()) {
            words[b] = new Word(b, Tag::ID);
            return static_cast<Token *>(words[b]);
        } else
            return static_cast<Token *>(got->second);
    }
    auto tok = new Token(peek);
    peek = ' ';
    return tok;
}
