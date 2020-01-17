#include <utility>

//
// Created by rileywen on 11/16/18.
//

#ifndef TOYCOMPILER_ASTNODE_H
#define TOYCOMPILER_ASTNODE_H

#include <exception>
#include <string>
#include <memory>
#include <list>
#include <myparser.hh>
#include "token.h"

struct Node {
    virtual void print(int level) = 0;

    void printws(int level) {
        for (int i = 0; i < level; i++)
            cout << "  ";
    };
};

struct Expr : Node {
    ptr<Token> tok;

    virtual void print(int level) {};

    Expr(ptr<Token> tok) : tok(std::move(tok)) {}
};

struct Id : Expr {
    Id(ptr<Word> id) : Expr(id) {}

    void print(int level) override;
};

// Nodes for terminals
struct Constant : Expr {
    Constant(int i) : Expr(new_ptr<Num>(i)) {}

    Constant(std::string str) : Expr(new_ptr<Word>(str, Tag::STRING)) {}

    void print(int level) override;
};

struct ExprList : Expr {
    std::list<ptr<Expr>> exprlist;

    void print(int level) override;

    ExprList(std::list<ptr<Expr>> exprlist)
            : Expr(new_ptr<Token>(Tag::DESC)), exprlist(std::move(exprlist)) {}
};

// For primary_expr
struct FuncCall : Expr {
    ptr<Id> id;
    ptr<ExprList> exprlist;

    void print(int level) override;;

    FuncCall(ptr<Id> id, ptr<ExprList> exprlist = nullptr)
            : Expr(new_ptr<Word>("FuncCall", Tag::DESC)), id(id), exprlist(exprlist) {}
};

struct Access : Expr {
    ptr<Id> array;
    ptr<Expr> index;

    virtual void print(int level);

    Access(ptr<Id> array, ptr<Expr> index)
            : Expr(new_ptr<Word>("[]", Tag::INDEX)), array(array) {}
};

struct SetElem : Expr {
    ptr<Access> access;
    ptr<Expr> expr;

    virtual void print(int level);

    SetElem(ptr<Access> access, ptr<Expr> expr)
            : Expr(new_ptr<Word>("[]=", Tag::DESC)), access(access), expr(expr) {}
};

struct Set : Expr {
    ptr<Id> id;
    ptr<Expr> expr;

    virtual void print(int level);

    Set(ptr<Id> id, ptr<Expr> expr)
            : Expr(new_ptr<Word>("Set", Tag::DESC)), id(id), expr(expr) {}
};

struct Binary : Expr {
    ptr<Expr> expr1, expr2;

    void print(int level) override;

    Binary(ptr<Token> Op, ptr<Expr> expr1, ptr<Expr> expr2)
            : Expr(Op), expr1(expr1), expr2(expr2) {}
};

struct Unary : Expr {
    ptr<Expr> expr;

    Unary(ptr<Token> tok, ptr<Expr> expr)
            : Expr(tok), expr(expr) {}
};

// For Declaration
struct Decl : Node {
    typedef enum {
        ARRAY, FUNC, ID, PARAM, PARAM_LIST, DECL_LIST
    } DECLTYPE;
    DECLTYPE DeclType;

    virtual void print(int level) {}

    Decl(Decl::DECLTYPE DeclType) : Node(), DeclType(DeclType) {}
};

struct Decls : Decl {
    std::list<ptr<Decl>> decl_list;

    Decls(std::list<ptr<Decl>> decl_list)
            : Decl(Decl::DECL_LIST), decl_list(decl_list) {}
};

struct ConstantList : Expr {
    std::list<ptr<Constant>> constlist;

    ConstantList(std::list<ptr<Constant>> constlist)
            : Expr(new_ptr<Word>("ConstList", Tag::DESC)), constlist(constlist) {}
};

struct Parameter : Decl {
    ptr<Type> type;
    ptr<Id> id;

    virtual void print(int level);

    Parameter(ptr<Type> type, ptr<Id> id) : Decl(Decl::PARAM), type(type), id(id) {}
};

struct ParameterList : Decl {
    std::list<ptr<Parameter>> parameter_list;

    ParameterList(std::list<ptr<Parameter>> param_list)
            : Decl(Decl::PARAM_LIST), parameter_list(param_list) {}
};

struct ArrayDecl : Decl {
    ptr<Expr> size;
    ptr<Id> id;
    ptr<ConstantList> constlist;


    void print(int level) override;

    ArrayDecl(ptr<Id> id, ptr<Expr> size = nullptr, ptr<ConstantList> constlist = nullptr)
            : Decl(DECLTYPE::ARRAY), size(size), id(id), constlist(constlist) {}
};

struct IdDecl : Decl {
    ptr<Id> id_name;
    ptr<Expr> default_val;

    virtual void print(int level);

    IdDecl(ptr<Id> id_name, ptr<Expr> default_val = nullptr)
            : Decl(Decl::ID), id_name(id_name), default_val(default_val) {}
};

struct IdList : Expr {
    std::list<ptr<Id>> ids;

    IdList(std::list<ptr<Id>> ids)
            : Expr(new_ptr<Word>("IdList", Tag::DESC)), ids(ids) {}

    virtual void print(int level);
};

// For statements
struct Stmt : Node {
    virtual void print(int level) {};
    static ptr<Stmt> Null;
};


struct StmtList : Stmt {
    std::list<ptr<Stmt>> stmt_list;

    void print(int level) override;

    StmtList(std::list<ptr<Stmt>> stmt_list)
            : Stmt(), stmt_list(stmt_list) {}
};

struct If : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt;

    void print(int level) override;

    If(ptr<Expr> expr, ptr<Stmt> stmt)
            : Stmt(), expr(expr), stmt(stmt) {}
};

struct Else : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt1;
    ptr<Stmt> stmt2;

    void print(int level) override;

    Else(ptr<Expr> expr, ptr<Stmt> stmt1, ptr<Stmt> stmt2)
            : Stmt(), expr(expr), stmt1(stmt1), stmt2(stmt2) {}
};

struct While : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt;

    void print(int level) override;

    While(ptr<Expr> expr, ptr<Stmt> stmt)
            : Stmt(), expr(expr), stmt(stmt) {}
};

struct Return : Stmt {
    ptr<Expr> return_address_expr;

    void print(int level) override;

    Return(ptr<Expr> return_address_expr = nullptr)
            : Stmt(), return_address_expr(std::move(return_address_expr)) {}
};

struct Print : Stmt {
    ptr<ExprList> expr_list;

    void print(int level) override;

    Print(ptr<ExprList> expr_list = nullptr)
            : Stmt(), expr_list(std::move(expr_list)) {}
};

struct Scan : Stmt {
    ptr<IdList> idlist;

    Scan(ptr<IdList> idlist) : Stmt(), idlist(std::move(idlist)) {}
};

struct FunctionDecl : Decl {    // no print()
    ptr<Id> id;
    ptr<ParameterList> param_list;

    FunctionDecl(ptr<Id> id, ptr<ParameterList> param_list = nullptr)
            : Decl(Decl::FUNC), id(std::move(id)), param_list(std::move(param_list)) {}
};

struct Function : Stmt {
    ptr<Type> type;
    ptr<FunctionDecl> func_decl;
    ptr<Stmt> block;

    virtual void print(int level);

    Function(ptr<Type> type, ptr<FunctionDecl> func_decl, ptr<Stmt> block)
            : Stmt(), type(type), func_decl(func_decl), block(block) {}
};

struct VarDeclList : Stmt {
    ptr<Type> type;
    ptr<Decls> decl_list;

    virtual void print(int level);;

    VarDeclList(ptr<Type> type, ptr<Decls> decl_list)
            : Stmt(), type(type), decl_list(decl_list) {}
};

struct ExternDecls : Stmt {
    std::list<ptr<Stmt>> stmt_list;

    virtual void print(int level) {
        printws(level);
        cout << "External Declarations:\n";
        for (auto &&i : stmt_list)
            i->print(level + 1);
    };

    ExternDecls(std::list<ptr<Stmt>> stmt_list) : Stmt(), stmt_list(stmt_list) {}
};

struct ExprStmt : Stmt {
    ptr<Expr> expr;

    virtual void print(int level);

    ExprStmt(ptr<Expr> expr) : Stmt(), expr(expr) {}
};

#endif //TOYCOMPILER_ASTNODE_H
