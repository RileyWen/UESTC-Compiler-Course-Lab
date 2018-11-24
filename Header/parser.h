//
// Created by rileywen on 11/16/18.
//

#ifndef TOYCOMPILER_PARSER_H
#define TOYCOMPILER_PARSER_H

#include <algorithm>
#include <exception>
#include <vector>
#include <string>
#include <array>
#include "lexer.h"
#include "Util/token.h"
#include "Util/AstNode.h"

class ParserException : public std::exception {
private:
    std::string exp_msg;
public:
    explicit ParserException(std::string &&exp_msg) : exp_msg(exp_msg) {}

    const char *what() const noexcept {
        return exp_msg.c_str();
    }
};

class Parser {
private:
    ptr<Lexer> Lex;
    ptr<Token> look;

    void move() { look = Lex->scan(); }

    void error(std::string &&s);

    template<typename... T>
    ptr<Token> match(T... Args);

    ptr<Stmt> external_declaration();

    ptr<Decls> declarator_list(ptr<Type> type);

    ptr<ConstantList> constant_list();

    ptr<Decl> declarator(ptr<Type> type);

    ptr<Node> decl_or_stmt(ptr<Type> type);

    ptr<ParameterList> parameter_list();

    ptr<Stmt> statement();

    ptr<StmtList> statement_list();

    ptr<Expr> expr();

    ptr<Expr> add_expr();

    ptr<Expr> mul_expr();

    ptr<Expr> primary_expr();

    ptr<ExprList> expr_list();

    ptr<IdList> id_list();

    ptr<While> while_stmt();

    ptr<Return> return_stmt();

    ptr<Scan> scan_stmt();

    ptr<Print> print_stmt();

public:
    Parser(ptr<Lexer> l) : Lex(l) { move(); };

    ptr<Stmt> program();
};

#endif //TOYCOMPILER_PARSER_H
