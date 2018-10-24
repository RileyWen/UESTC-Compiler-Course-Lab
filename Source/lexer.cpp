//
// Created by rileywen on 9/24/18.
//
#include "lexer.h"
#include <cctype>
#include <string>

int Lexer::line = 1;

std::shared_ptr<Token> Lexer::scan() {
    for (readch();; readch()) {
        if (peek == ' ' || peek == '\t' || peek == '\n') continue;
        else if (peek == std::char_traits<char>::eof()) return nullptr;
        else if (peek == '#') while (readch(), peek != '\n') {}
        else if (peek == '\'') {
            readch();
            if (peek == '\'') {
                if (nextch('\'')) {
                    readch();
                    while (true) {
                        if (readch(), peek == '\'')
                            if (readch(), peek == '\'')
                                if (readch(), peek == '\'')
                                    break;
                    }
                } else return std::make_shared<Word>(std::string(""), Tag::STR);
            } else {
                std::string buffer;
                buffer += peek;
                while (readch(), peek != '\'')
                    buffer += peek;
                return std::make_shared<Word>(std::move(buffer), Tag::STR);
            }
        } else if (peek == '\"') {
            readch();
            if (peek == '\"') {
                if (nextch('\"')) {
                    readch();
                    while (true) {
                        if (readch(), peek == '\"')
                            if (readch(), peek == '\"')
                                if (readch(), peek == '\"')
                                    break;
                    }
                } else return std::make_shared<Word>(std::string(""), Tag::STR);
            } else {
                std::string buffer;
                buffer += peek;
                while (readch(), peek != '\"')
                    buffer += peek;
                return std::make_shared<Word>(std::move(buffer), Tag::STR);
            }
        } else break;
    }
    switch (peek) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
        case ';':
        case '.':
        case ':':
        case '[':
        case ']':
        case ',':
            return std::make_shared<Token>(peek);
        case '=':
            if (nextch('=')) {
                readch();
                return std::make_shared<Word>(std::string("=="), Tag::EQ);
            } else
                return std::make_shared<Token>('=');
        case '<':
            if (nextch('=')) {
                readch();
                return std::make_shared<Word>(std::string("<="), Tag::LE);
            } else
                return std::make_shared<Token>('<');
        case '>':
            if (nextch('=')) {
                readch();
                return std::make_shared<Word>(std::string(">="), Tag::GE);
            } else
                return std::make_shared<Token>('>');
        case '!':
            if (nextch('=')) {
                readch();
                return std::make_shared<Word>(std::string("!="), Tag::NE);
            } else
                return std::make_shared<Token>('>');
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
            return std::make_shared<Num>(v);
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
        return std::make_shared<Real>(x);
    } else if (std::isalpha(peek)) {
        std::string b;
        do {
            b += peek;
            readch();
        } while (std::isalpha(peek) || std::isdigit(peek) || peek == '_');
        retract();

        auto got = words.find(b);
        if (got == words.end()) {
            words[b] = std::make_shared<Word>(b, Tag::ID);
            return words[b];
        } else
            return got->second;
    } else {
        std::cout << "<Unknown:" << std::hex << (int) peek << ">";
        return std::make_shared<Token>(Tag::UNKNOWN);
    }
}

void Lexer::readch() {
    std::cin.get(peek);
    if (peek == '\n') line++;
    if (std::cin.eof())
        throw LexerException("EOF");
}