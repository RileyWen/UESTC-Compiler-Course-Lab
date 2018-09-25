#include <iostream>
#include <memory>
#include "lexer.h"

int main() {
    Lexer lexer;
    Token *tok;
    while (tok = lexer.scan(), tok) {
        std::cout << tok->tag << " ";
    }
    return 0;
}