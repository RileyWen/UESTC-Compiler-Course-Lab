//
// Created by rileywen on 11/16/18.
//

#ifndef TOYCOMPILER_ASTNODE_H
#define TOYCOMPILER_ASTNODE_H

#include <exception>
#include <string>
#include <memory>
#include <list>
#include "token.h"

struct Node {
    int level;

    explicit Node(int level) : level(level) {}

    virtual void print(int level) = 0;

    void printws(int level) {
        for (int i = 0; i < level; i++)
            cout << "  ";
    };
};

struct Expr : Node {
    ptr<Token> tok;

    virtual void print(int level) {};

    Expr(int level, ptr<Token> tok) : Node(level), tok(tok) {}
};

struct Id : Expr {
    Id(int level, ptr<Word> id) : Expr(level, id) {}

    void print(int level) override;
};

// Nodes for terminals
struct Constant : Expr {
    Constant(int level, int i) : Expr(level, new_ptr<Num>(i)) {}

    Constant(int level, std::string str) : Expr(level, new_ptr<Word>(str, Tag::STRING)) {}

    void print(int level) override;
};

struct ExprList : Expr {
    std::list<ptr<Expr>> exprlist;

    void print(int level) override;

    ExprList(int level, std::list<ptr<Expr>> exprlist)
            : Expr(level, new_ptr<Token>(Tag::DESC)), exprlist(std::move(exprlist)) {}
};

// For primary_expr
struct FuncCall : Expr {
    ptr<Id> id;
    ptr<ExprList> exprlist;

    void print(int level) override;;

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

    void print(int level) override;

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

    virtual void print(int level) {}

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

    virtual void print(int level) {
        printws(level);
        cout << "Type: " << type->lexeme << "  Name: "
             << ptr_to<Word>(id->tok)->lexeme << "\n";
    }

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


    void print(int level) override {
        printws(level);
        cout << "Array Name: " << ptr_to<Word>(id->tok)->lexeme << "\n";
        printws(level);
        if (size) {
            cout << "Size Expression:\n";
            size->print(level + 1);
        } else
            cout << "Size Expression is not specified\n";
        printws(level);
        if (constlist) {
            cout << "Constant List:\n";
            for (auto &&i : constlist->constlist)
                i->print(level + 1);
        } else
            cout << "Constant List is not specified";
    }

    ArrayDecl(int level, ptr<Id> id, ptr<Expr> size = nullptr, ptr<ConstantList> constlist = nullptr)
            : Decl(level, DECLTYPE::ARRAY), size(size), id(id), constlist(constlist) {}
};

struct IdDecl : Decl {
    ptr<Id> id_name;
    ptr<Expr> default_val;

    virtual void print(int level) {
        printws(level);
        cout << "Identifier Declaration:\n";
        printws(level + 1);
        cout << "Identifier Name: " << ptr_to<Word>(id_name->tok)->lexeme << "\n";
        printws(level + 1);
        if (default_val) {
            cout << "Default Value Expr:\n";
            default_val->print(level + 2);
        } else {
            cout << "Default Value is not specified\n";
        }
    }

    IdDecl(int level, ptr<Id> id_name, ptr<Expr> default_val = nullptr)
            : Decl(level, Decl::ID), id_name(id_name), default_val(default_val) {}
};

struct IdList : Expr {
    std::list<ptr<Id>> ids;

    IdList(int level, std::list<ptr<Id>> ids)
            : Expr(level, new_ptr<Word>("IdList", Tag::DESC)), ids(ids) {}

    virtual void print(int level) {
        for (auto id : ids) {
            printws(level);
            cout << "Identifier<" << id->tok << ">\n";
        }
    }
};

// For statements
struct Stmt : Node {
    Stmt(int level) : Node(level) {}

    virtual void print(int level) {};
    static ptr<Stmt> Null;
};


struct StmtList : Stmt {
    std::list<ptr<Stmt>> stmt_list;

    void print(int level) override;

    StmtList(int level, std::list<ptr<Stmt>> stmt_list)
            : Stmt(level), stmt_list(stmt_list) {}
};

struct If : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt;

    void print(int level) override;

    If(int level, ptr<Expr> expr, ptr<Stmt> stmt)
            : Stmt(level), expr(expr), stmt(stmt) {}
};

struct Else : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt1;
    ptr<Stmt> stmt2;

    void print(int level) override;

    Else(int level, ptr<Expr> expr, ptr<Stmt> stmt1, ptr<Stmt> stmt2)
            : Stmt(level), expr(expr), stmt1(stmt1), stmt2(stmt2) {}
};

struct While : Stmt {
    ptr<Expr> expr;
    ptr<Stmt> stmt;

    void print(int level) override;

    While(int level, ptr<Expr> expr, ptr<Stmt> stmt)
            : Stmt(level), expr(expr), stmt(stmt) {}
};

struct Return : Stmt {
    ptr<Expr> return_address_expr;

    void print(int level) override;

    Return(int level, ptr<Expr> return_address_expr = nullptr)
            : Stmt(level), return_address_expr(std::move(return_address_expr)) {}
};

struct Print : Stmt {
    ptr<ExprList> expr_list;

    void print(int level) override;

    Print(int level, ptr<ExprList> expr_list = nullptr)
            : Stmt(level), expr_list(std::move(expr_list)) {}
};

struct Scan : Stmt {
    ptr<IdList> idlist;

    Scan(int level, ptr<IdList> idlist) : Stmt(level), idlist(std::move(idlist)) {}
};

struct FunctionDecl : Decl {    // no print()
    ptr<Id> id;
    ptr<ParameterList> param_list;

    FunctionDecl(int level, ptr<Id> id, ptr<ParameterList> param_list = nullptr)
            : Decl(level, Decl::FUNC), id(std::move(id)), param_list(std::move(param_list)) {}
};

struct Function : Stmt {
    ptr<Type> type;
    ptr<FunctionDecl> func_decl;
    ptr<Stmt> block;

    virtual void print(int level);

    Function(int level, ptr<Type> type, ptr<FunctionDecl> func_decl, ptr<Stmt> block)
            : Stmt(level), type(type), func_decl(func_decl), block(block) {}
};

struct VarDeclList : Stmt {
    ptr<Type> type;
    ptr<Decls> decl_list;

    virtual void print(int level) {
        printws(level);
        cout << "Type " << type->lexeme << ":\n";
        for (auto &&i : decl_list->decl_list)
            i->print(level + 1);
    };

    VarDeclList(int level, ptr<Type> type, ptr<Decls> decl_list)
            : Stmt(level), type(type), decl_list(decl_list) {}
};

struct ExternDecls : Stmt {
    std::list<ptr<Stmt>> stmt_list;

    virtual void print(int level) {
        printws(level);
        cout << "External Declarations:\n";
        for (auto &&i : stmt_list)
            i->print(level + 1);
    };

    ExternDecls(int level, std::list<ptr<Stmt>> stmt_list) : Stmt(level), stmt_list(stmt_list) {}
};

struct ExprStmt : Stmt {
    ptr<Expr> expr;

    ExprStmt(int level, ptr<Expr> expr) : Stmt(level), expr(expr) {}
};

#endif //TOYCOMPILER_ASTNODE_H
