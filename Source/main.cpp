#include <iostream>
#include <memory>
#include "lexer.h"

int main() {
    Lexer lexer;
    Token *token;
    while (token = lexer.scan(), token) {
        if (token->tag < 256) {
            std::cout << "<" << (char) token->tag << ">\n";
            continue;
        }
        switch (token->tag) {
            case Tag::ID:
                std::cout << "<ID, \"" << static_cast<Word *>(token)->lexeme << "\">";
                break;
            case Tag::UNKNOWN:
                std::cout << "<UNKNOWN>";
                break;
            default:
                break;
        }
    }
    return 0;
}