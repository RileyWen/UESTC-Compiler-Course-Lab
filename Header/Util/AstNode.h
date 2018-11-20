//
// Created by rileywen on 11/16/18.
//

#ifndef TOYCOMPILER_ASTNODE_H
#define TOYCOMPILER_ASTNODE_H

#include <exception>
#include <string>
#include <memory>
#include <iostream>
#include <list>
#include "token.h"

struct Node {
    int level;

    Node(int level) : level(level) {}

    virtual void print() {}

    void printws() {
        for (int i = 0; i < level; i++)
            std::cout << "\t";
    };
};

struct Expr : Node {
    ptr<Token> tok;

    Expr(int level, ptr<Token> tok) : Node(level), tok(tok) {}
};

struct Id : Expr {
    Id(int level, ptr<Word> id) : Expr(level, id) {}
};

// Nodes for terminals
struct Constant : Expr {
    Constant(int level, int i) : Expr(level, new_ptr<Num>(i)) {}

    Constant(int level, std::string str) : Expr(level, new_ptr<Word>(str, Tag::STRING)) {}
};

struct ExprList : Expr {
    std::list<ptr<Expr>> exprlist;

    ExprList(int level, std::list<ptr<Expr>> exprlist)
            : Expr(level, new_ptr<Token>(Tag::DESC)), exprlist(exprlist) {}
};

// For primary_expr
struct FuncCall : Expr {
    ptr<Id> id;
    ptr<ExprList> exprlist;

    FuncCall(int level, ptr<Id> id, ptr<ExprList> exprlist = nullptr)
            : Expr(level, new_ptr<Word>("FuncCall", Tag::DESC)), id(id), exprlist(exprlist) {}
};

struct Access : Expr {
    ptr<Id> array;
    ptr<Expr> index;

    Access(int level, ptr<Id> array, ptr<Expr> index)
            : Expr(level, new_ptr<Word>("[]", Tag::INDEX)) {}
};

struct SetElem : Expr {
    ptr<Access> access;
    ptr<Expr> expr;

    SetElem(int level, ptr<Access> access, ptr<Expr> expr)
            : Expr(level, new_ptr<Word>("[]=", Tag::DESC)), access(access), expr(expr) {}
};

struct Set : Expr {
    ptr<Id> id;
    ptr<Expr> expr;

    Set(int level, ptr<Id> id, ptr<Expr> expr)
            : Expr(level, new_ptr<Word>("Set", Tag::DESC)), id(id), expr(expr) {}
};

struct Binary : Expr {
    ptr<Expr> expr1, expr2;

    Binary(int level, ptr<Token> Op, ptr<Expr> expr1, ptr<Expr> expr2)
            : Expr(level, Op), expr1(expr1), expr2(expr2) {}
};

struct Unary : Expr {
    ptr<Expr> expr;

    Unary(int level, ptr<Token> tok, ptr<Expr> expr)
            : Expr(level, tok), expr(expr) {}
};

// For Declaration
struct Decl : Node {
    typedef enum {
        ARRAY, FUNC, ID, PARAM, PARAM_LIST, DECL_LIST
    } DECLTYPE;
    DECLTYPE DeclType;

    Decl(int level, Decl::DECLTYPE DeclType) : Node(level), DeclType(DeclType) {}
};

struct Decls : Decl {
    std::list<ptr<Decl>> decl_list;

    Decls(int level, std::list<ptr<Decl>> decl_list)
            : Decl(level, Decl::DECL_LIST), decl_list(decl_list) {}
};

struct ConstantList : Expr {
    std::list<ptr<Constant>> constlist;

    ConstantList(int level, std::list<ptr<Constant>> constlist)
            : Expr(level, new_ptr<Word>("ConstList", Tag::DESC)), constlist(constlist) {}
};

struct Parameter : Decl {
    ptr<Type> type;
    ptr<Id> id;

    Parameter(int level, ptr<Type> type, ptr<Id> id) : Decl(level, Decl::PARAM), type(type), id(id) {}
};

struct ParameterList : Decl {
    std::list<ptr<Parameter>> parameter_list;

    ParameterList(int level, std::list<ptr<Parameter>> param_list)
            : Decl(level, Decl::PARAM_LIST), parameter_list(param_list) {}
};

struct ArrayDecl : Decl {
    ptr<Expr> size;
    ptr<Id> id;
    ptr<ConstantList> constlist;

    ArrayDecl(int level, ptr<Id> id, ptr<Expr> size = nullptr, ptr<ConstantList> constlist = nullptr)
            : Decl(level, DECLTYPE::ARRAY), size(size), id(id), constlist(constlist) {}
};

struct IdDecl : Decl {
    ptr<Id> id_name;
    ptr<Expr> default_val;

    IdDecl(int level, ptr<Id> id_name, ptr<Expr> default_val = nullptr)
            : Decl(level, Decl::ID), id_name(id_name), default_val(default_val) {}
};

struct IdList : Expr {
    std::list<ptr<Id>> ids;

    IdList(int level, std::list<ptr<Id>> ids)
            : Expr(level, new_ptr<Word>("IdList", Tag::DESC)), ids(ids) {}

    virtual void print() {
        for (auto id : ids) {
            printws();
            std::cout << "Identifier<" << id->tok << ">\n";
        }
    }
};

// For statements
struct Stmt : Node {
    Stmt(int level) : Node(level) {}

    static ptr<Stmt> Null;
};


struct StmtList : Stmt {
    std::list<ptr<Stmt>> stmt_list;

    StmtList(int level, std::list<ptr<Stmt>> stmt_list)
            : Stmt(level), stmt_list(stmt_list) {}
};

struct If : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt;

    If(int level, ptr<Expr> expr, ptr<Stmt> stmt)
            : Stmt(level), expr(expr), stmt(stmt) {}
};

struct Else : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt1;
    ptr<Stmt> stmt2;

    Else(int level, ptr<Expr> expr1, ptr<Stmt> stmt1, ptr<Stmt> stmt2)
            : Stmt(level), expr(expr), stmt1(stmt1), stmt2(stmt2) {}
};

struct While : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt;

    While(int level, ptr<Expr> expr, ptr<Stmt> stmt)
            : Stmt(level), expr(expr), stmt(stmt) {}
};

struct Return : Stmt {
    ptr<Expr> return_address_expr;

    Return(int level, ptr<Expr> return_address_expr = nullptr)
            : Stmt(level), return_address_expr(return_address_expr) {}
};

struct Print : Stmt {
    ptr<ExprList> expr_list;

    Print(int level, ptr<ExprList> expr_list = nullptr)
            : Stmt(level), expr_list(expr_list) {}
};

struct Scan : Stmt {
    ptr<IdList> idlist;

    Scan(int level, ptr<IdList> idlist) : Stmt(level), idlist(idlist) {}
};

struct Function : Stmt {
    ptr<Type> type;
    ptr<Decl> func_decl;
    ptr<Stmt> block;

    Function(int level, ptr<Type> type, ptr<Decl> func_decl, ptr<Stmt> block)
            : Stmt(level), type(type), func_decl(func_decl), block(block) {}
};

struct FunctionDecl : Decl {
    ptr<Id> id;
    ptr<ParameterList> param_list;

    FunctionDecl(int level, ptr<Id> id, ptr<ParameterList> param_list = nullptr)
            : Decl(level, Decl::FUNC), id(id), param_list(param_list) {}
};

struct VarDeclList : Stmt {
    ptr<Type> type;
    ptr<Decls> decl_list;

    VarDeclList(int level, ptr<Type> type, ptr<Decls> decl_list)
            : Stmt(level), type(type), decl_list(decl_list) {}
};

struct ExternDecls : Stmt {
    std::list<ptr<Stmt>> stmt_list;

    ExternDecls(int level, std::list<ptr<Stmt>> stmt_list) : Stmt(level), stmt_list(stmt_list) {}
};

struct ExprStmt : Stmt {
    ptr<Expr> expr;

    ExprStmt(int level, ptr<Expr> expr) : Stmt(level), expr(expr) {}
};

#endif //TOYCOMPILER_ASTNODE_H
