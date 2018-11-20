//
// Created by rileywen on 11/19/18.
//

#include <Util/AstNode.h>

#include "Util/AstNode.h"

ptr<Stmt> Stmt::Null = new_ptr<Stmt>(0);

void Function::print(int level) {
    printws(level);
    cout << "Function Declaration:\n";
    printws(level + 1);
    cout << "Function Name: " << ptr_to<Word>(func_decl->id->tok)->lexeme << "\n";
    printws(level + 1);
    if (func_decl->param_list) {
        cout << "Parameter List:\n";
        for (auto &&i : func_decl->param_list->parameter_list)
            i->print(level + 2);
    } else
        cout << "No Parameter\n";
    printws(level + 1);
    cout << "Statement Block:\n";
    block->print(level + 2);
}

void Print::print(int level) {
    printws(level);
    cout << "Print:\n";
    expr_list->print(level + 1);
}

void While::print(int level) {
    printws(level);
    cout << "While:\n";
    printws(level + 1);
    cout << "Condition Expression:\n";
    expr->print(level + 2);
    printws(level + 1);
    cout << "Statement:\n";
    stmt->print(level + 2);
}

void StmtList::print(int level) {
    printws(level);
    cout << "Statement List:\n";
    for (auto &&i : stmt_list)
        i->print(level + 1);
}

void Return::print(int level) {
    printws(level);
    cout << "Return:\n";
    printws(level + 1);
    if (!return_address_expr)
        cout << "No Return Value\n";
    else {
        cout << "Ret Addr Expr:\n";
        return_address_expr->print(level + 2);
    }
}

void ExprList::print(int level) {
    printws(level);
    cout << "Expr List:\n";
    for (auto &&i : exprlist)
        i->print(level + 1);
}

void Constant::print(int level) {
    printws(level);
    cout << "Constant:\n";
    printws(level + 1);
    if (tok->tag == Tag::STRING) {
        cout << "Type: String\n";
        printws(level + 1);
        cout << "Value: \"" << ptr_to<Word>(tok)->lexeme << "\"\n";
    } else {
        cout << "Type: Int\n";
        printws(level + 1);
        cout << "Value: " << ptr_to<Num>(tok)->value << "\n";
    }
}

void Binary::print(int level) {
    printws(level);
    cout << "OP Token: ";
    if (tok->tag < 256)
        cout << (char) tok->tag << "\n";
    else
        cout << ptr_to<Word>(tok)->lexeme << "\n";
    printws(level + 1);
    cout << "Expr 1:\n";
    expr1->print(level + 2);
    printws(level + 1);
    cout << "Expr 2:\n";
    expr2->print(level + 2);
}

void Id::print(int level) {
    printws(level);
    cout << "Id Ref: " << ptr_to<Word>(tok)->lexeme << "\n";
}

void If::print(int level) {
    printws(level);
    cout << "If:\n";
    printws(level + 1);
    cout << "Condition Expr:\n";
    expr->print(level + 2);
    printws(level + 1);
    cout << "True Stmt:\n";
    stmt->print(level + 2);
}

void Else::print(int level) {
    printws(level);
    cout << "If Else:\n";
    printws(level + 1);
    cout << "Condition Expr:\n";
    expr->print(level + 2);
    printws(level + 1);
    cout << "True Stmt:\n";
    stmt1->print(level + 2);
    printws(level + 1);
    cout << "False Stmt:\n";
    stmt2->print(level + 2);
}

void FuncCall::print(int level) {
    printws(level);
    cout << "Func Call:\n";
    printws(level + 1);
    cout << "Func Name: " << ptr_to<Word>(id->tok)->lexeme << "\n";
    printws(level + 1);
    if (exprlist) {
        cout << "Expr List of Func Args:\n";
        exprlist->print(level + 2);
    } else {
        cout << "No Args\n";
    }
}
