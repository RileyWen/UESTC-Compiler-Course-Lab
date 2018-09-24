//
// Created by rileywen on 9/24/18.
//
#include "lexer.h"
#include <cctype>

bool Lexer::readch(char c) {

}

Token *Lexer::scan() {
    for (;; readch()) {
        if (peek == ' ' || peek == '\t') continue;
        else if (peek == '\n') line++;
        else break;
    }
    switch (peek) {
        case '&':
            if ()
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
