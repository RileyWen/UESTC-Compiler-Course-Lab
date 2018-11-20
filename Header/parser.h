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

    ptr<Stmt> external_declaration(int level);

    ptr<Decls> declarator_list(int level, ptr<Type> type);

    ptr<ConstantList> constant_list(int level);

    ptr<Decl> declarator(int level, ptr<Type> type);

    ptr<Node> decl_or_stmt(int level, ptr<Type> type);

    ptr<ParameterList> parameter_list(int level);

    ptr<Stmt> statement(int level);

    ptr<StmtList> statement_list(int level);

    ptr<Expr> expr(int level);

    ptr<Expr> add_expr(int level);

    ptr<Expr> mul_expr(int level);

    ptr<Expr> primary_expr(int level);

    ptr<ExprList> expr_list(int level);

    ptr<IdList> id_list(int level);

    ptr<While> while_stmt(int level);

    ptr<Return> return_stmt(int level);

    ptr<Scan> scan_stmt(int level);

    ptr<Print> print_stmt(int level);

public:
    Parser(ptr<Lexer> l) : Lex(l) { move(); };

    ptr<Stmt> program(int level);
};

#endif //TOYCOMPILER_PARSER_H
