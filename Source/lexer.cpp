//
// Created by rileywen on 9/24/18.
//
#include "lexer.h"
#include <cctype>
#include <string>

int Lexer::line = 1;

Token *Lexer::scan() {
    for (readch();; readch()) {
        if (peek == ' ' || peek == '\t') continue;
        else if (peek == '\n') line++;
        else if (peek == std::char_traits<char>::eof()) return nullptr;
        else break;
    }
    switch (peek) {
        case '+':
        case '-':
        case '(':
        case ')':
        case ';':
            return new Token(peek);
        case '=':
            if (nextch('=')) {
                readch();
                return static_cast<Token *>(new Word(std::string("=="), Tag::EQ));
            } else
                return new Token('=');
        case '<':
            if (nextch('=')) {
                readch();
                return static_cast<Token *>(new Word(std::string("=="), Tag::LE));
            } else
                return new Token('<');
        case '>':
            if (nextch('=')) {
                readch();
                return static_cast<Token *>(new Word(std::string("=="), Tag::GE));
            } else
                return new Token('>');
        default:
            break;
    }
    if (std::isdigit(peek)) {
        int v = 0;
        do {
            v = 10 * v + ctoi(peek);
            readch();
        } while (std::isdigit(peek));
        if (peek != '.') {
            retract();
            return static_cast<Token *>(new Num(v));
        }
        double x = v, d = 10;
        for (;;) {
            readch();
            if (!std::isdigit(peek)) {
                retract();
                break;
            }
            x += (double) ctoi(peek) / d;
            d *= 10;
        }
        return static_cast<Token *>(new Real(x));
    } else if (std::isalpha(peek)) {
        std::string b;
        do {
            b += peek;
            readch();
        } while (std::isalpha(peek) || std::isdigit(peek));
        retract();

        auto got = words.find(b);
        if (got == words.end()) {
            words[b] = new Word(b, Tag::ID);
            return static_cast<Token *>(words[b]);
        } else
            return static_cast<Token *>(got->second);
    } else
        return new Token(Tag::UNKNOWN);
}
