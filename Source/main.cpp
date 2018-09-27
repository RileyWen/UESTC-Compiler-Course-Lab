#include <iostream>
#include <fstream>
#include <memory>
#include "lexer.h"
#include <unistd.h>

int main() {
    std::ifstream in("/home/rileywen/Downloads/python.py");
    if (in.is_open()) {
        std::cin.rdbuf(in.rdbuf());
        std::cout << "Redirect Successfully.\n";
    }

    Lexer lexer;
    Token *token;
    try {
        while (token = lexer.scan(), token) {
            if (token->tag < 256) {
                std::cout << "<" << (char) token->tag << "> ";
                continue;
            }
            switch (token->tag) {
                case Tag::NUM:
                    std::cout << "<NUM, \"" << static_cast<Num *>(token)->value << "\"> ";
                    break;
                case Tag::REAL:
                    std::cout << "<REAL, \"" << static_cast<Num *>(token)->value << "\"> ";
                    break;
                case Tag::EQ:
                    std::cout << "<EQ, \"" << static_cast<Word *>(token)->lexeme << "\"> ";
                    break;
                case Tag::LE:
                    std::cout << "<LE, \"" << static_cast<Word *>(token)->lexeme << "\"> ";
                    break;
                case Tag::ID:
                    std::cout << "<ID, \"" << static_cast<Word *>(token)->lexeme << "\"> ";
                    break;
                case Tag::GE:
                    std::cout << "<GE, \"" << static_cast<Word *>(token)->lexeme << "\"> ";
                    break;
                case Tag::NE:
                    std::cout << "<NE, \"" << static_cast<Word *>(token)->lexeme << "\"> ";
                    break;
                case Tag::STR:
                    std::cout << "<STR, \"" << static_cast<Word *>(token)->lexeme << "\"> ";
                    break;
                default:
                    break;
            }
        }
    } catch (LexerException &E) {
        std::cout << E.what() << std::endl;
    }


    return 0;
}