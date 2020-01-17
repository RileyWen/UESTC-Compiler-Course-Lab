//
// Created by rileywen on 9/24/18.
//
#include "lexer.h"
#include <cctype>
#include <string>

extern "C" {
#include "lex.yy.c"
}

FlexAdapter::FlexAdapter(const char *path) {
    yyin = fopen(path, "r");
}


/*enum yytokentype{
    TYPE    = 257,  EOL     = 258,  STRING  = 259,  KEYWORD = 260,
    REL     = 261,  ID      = 262,  FLOAT   = 263,  ASSIGN  = 264,
    INT     = 265,  UNKNOWN = 266,  OP      = 268
};*/
ptr<Token> FlexAdapter::scan() {
    int token;
    while (token = yylex(), token == EOL)
        line++;
    if (token == 0)
        return new_ptr<Token>(Tag::EOF_);
    else if (token < 256) {
        printf("Char->\t%c\n", token);
        return new_ptr<Token>(token);
    } else {
        switch (token) {
            case TYPE:
                printf("TYPE->\t%s\n", yylval.strptr);
                return new_ptr<Type>(yylval.strptr, Tag::TYPE);
                break;
            case VOID:
                printf("VOID->\t%s\n", yylval.strptr);
                return new_ptr<Word>(yylval.strptr, Tag::VOID);
            case STRING:
                printf("STRING->\t%s\n", yylval.strptr);
                return new_ptr<Word>(yylval.strptr, Tag::STRING);
                break;
            case KEYWORD:
                printf("KEYWORD->\t%s\n", yylval.strptr);
                return new_ptr<Word>(yylval.strptr, Tag::KEYWORD);
                break;
            case REL:
                printf("REL->\t%s\n", yylval.strptr);
                return new_ptr<Word>(yylval.strptr, Tag::CMP);
                break;
            case ID:
                printf("ID->\t%s\n", yylval.strptr);
                return new_ptr<Word>(yylval.strptr, Tag::ID);
                break;
            case ASSIGN:
                printf("ASSIGN->\t%s\n", yylval.strptr);
                return new_ptr<Word>(yylval.strptr, Tag::ASSIGN);
                break;
            case INT:
                printf("INT->\t%d\n", yylval.intval);
                return new_ptr<Num>(yylval.intval);
                break;
            case OP:
                printf("OP->\t%d\n", yylval.intval);
                return new_ptr<Word>(yylval.strptr, Tag::OP);
            default:
                throw LexerException("UNKNOWN->\t" + std::string(yylval.strptr) + "\n");
//              return new_ptr<Token>(Tag::UNKNOWN);
        }
    }
}

std::shared_ptr<Token> MyLexer::scan() {
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
                } else return std::make_shared<Word>(std::string(""), Tag::STRING);
            } else {
                std::string buffer;
                buffer += peek;
                while (readch(), peek != '\'')
                    buffer += peek;
                return std::make_shared<Word>(std::move(buffer), Tag::STRING);
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
                } else return std::make_shared<Word>(std::string(""), Tag::STRING);
            } else {
                std::string buffer;
                buffer += peek;
                while (readch(), peek != '\"')
                    buffer += peek;
                return std::make_shared<Word>(std::move(buffer), Tag::STRING);
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
                return std::make_shared<Word>(std::string("=="), Tag::CMP);
            } else
                return std::make_shared<Token>('=');
        case '<':
            if (nextch('=')) {
                readch();
                return std::make_shared<Word>(std::string("<="), Tag::CMP);
            } else
                return std::make_shared<Token>('<');
        case '>':
            if (nextch('=')) {
                readch();
                return std::make_shared<Word>(std::string(">="), Tag::CMP);
            } else
                return std::make_shared<Token>('>');
        case '!':
            if (nextch('=')) {
                readch();
                return std::make_shared<Word>(std::string("!="), Tag::CMP);
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

void MyLexer::readch() {
    std::cin.get(peek);
    if (peek == '\n') line++;
    if (std::cin.eof())
        throw LexerException("EOF");
}