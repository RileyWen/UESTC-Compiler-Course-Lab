#include <iostream>
#include <fstream>
#include <memory>
#include "lexer.h"
#include <unistd.h>
#include <memory>
#include "parser.h"

void printtok(ptr<Token> tok) {
    ptr<Word> word;
    if (tok->tag < 256)
        std::cout << "<" << (char) tok->tag << "> ";
    else {
        std::cout << "<" << tok->tag;
        if (word = std::dynamic_pointer_cast<Word>(tok))
            std::cout << ", " << word->lexeme << "> ";
        else
            std::cout << "> ";
    }
}

int main() {
    ptr<Lexer> lexer = new_ptr<FlexAdapter>("/home/rileywen/Downloads/lab2/test.c");
    ptr<Parser> parser = new_ptr<Parser>(lexer);
    try {
        auto program_stmt = parser->program(0);
        /*ptr<Token> tok;
        while (true) {
            tok = lexer->scan();
            if (tok->tag==Tag::EOF_)
                break;
            //printtok(tok);
        }*/
    }
    catch (LexerException &E) {
        std::cout << E.what() << std::endl << lexer->line << std::endl;
    }
    catch (ParserException &E) {
        std::cout << E.what();
    }

    /*
    std::ifstream in("/home/rileywen/Downloads/python.py");
    if (in.is_open()) {
        std::cin.rdbuf(in.rdbuf());
        std::cout << "Redirect Successfully.\n";
    }
    MyLexer lexer;
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
                case Tag::STRING:
                    std::cout << "<STRING, \"" << std::dynamic_pointer_cast<Word>(token)->lexeme << "\"> ";
                    break;
                default:
                    break;
            }
        }
    } catch (LexerException &E) {
        std::cout << E.what() << std::endl;
    }*/
    return 0;
}