#include <iostream>
#include <fstream>
#include <memory>
#include "lexer.h"
#include <unistd.h>
#include <memory>

int main() {
    std::ifstream in("/home/rileywen/Downloads/python.py");
    if (in.is_open()) {
        std::cin.rdbuf(in.rdbuf());
        std::cout << "Redirect Successfully.\n";
    }

    Lexer lexer;
    std::shared_ptr<Token> token;
    try {
        while (token = lexer.scan(), token) {
            if (token->tag < 256) {
                std::cout << "<" << (char) token->tag << "> ";
                continue;
            }
            switch (token->tag) {
                case Tag::NUM:
                    std::cout << "<NUM, \"" << std::dynamic_pointer_cast<Num>(token)->value << "\"> ";
                    break;
                case Tag::REAL:
                    std::cout << "<REAL, \"" << std::dynamic_pointer_cast<Real>(token)->value << "\"> ";
                    break;
                case Tag::EQ:
                    std::cout << "<EQ, \"" << std::dynamic_pointer_cast<Word>(token)->lexeme << "\"> ";
                    break;
                case Tag::LE:
                    std::cout << "<LE, \"" << std::dynamic_pointer_cast<Word>(token)->lexeme << "\"> ";
                    break;
                case Tag::ID:
                    std::cout << "<ID, \"" << std::dynamic_pointer_cast<Word>(token)->lexeme << "\"> ";
                    break;
                case Tag::GE:
                    std::cout << "<GE, \"" << std::dynamic_pointer_cast<Word>(token)->lexeme << "\"> ";
                    break;
                case Tag::NE:
                    std::cout << "<NE, \"" << std::dynamic_pointer_cast<Word>(token)->lexeme << "\"> ";
                    break;
                case Tag::STR:
                    std::cout << "<STR, \"" << std::dynamic_pointer_cast<Word>(token)->lexeme << "\"> ";
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