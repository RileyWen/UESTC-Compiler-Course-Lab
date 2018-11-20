//
// Created by rileywen on 11/17/18.
//

#include "parser.h"

void Parser::error(std::string &&s) {
    throw ParserException(std::string("Parsing error occur near line ")
                          + std::to_string(Lex->line) + ":" + s + "\n");
}

template<typename... T>
ptr<Token> Parser::match(T... Args) {
    std::vector<int> match_list{Args...};
    ptr<Token> previous = look;
    if (std::find(match_list.begin(), match_list.end(), look->tag) != match_list.end()) {
        move();
        return previous;
    } else error("Syntax Error!");
}

ptr<Stmt> Parser::program(int level) {
    std::list<ptr<Stmt>> extr_decl_list;
    while (look->tag == Tag::TYPE || look->tag == Tag::VOID)
        extr_decl_list.push_back(external_declaration(level + 1));
    if (look->tag == Tag::EOF_)
        return new_ptr<ExternDecls>(level, std::move(extr_decl_list));
    else
        error("Unexpected Token with Tag " + std::to_string(look->tag));
}

ptr<Stmt> Parser::external_declaration(int level) {
    ptr<Decl> declarator_;
    ptr<Node> decl_or_stmt_;
    ptr<Type> type = ptr_to<Type>(std::move(look));
    move();
    declarator_ = declarator(level + 1, type);
    decl_or_stmt_ = decl_or_stmt(level + 1, type);
    ptr<Stmt> block;
    ptr<Decls> decl_list;
    if (block = ptr_to<Stmt>(decl_or_stmt_))
        return new_ptr<Function>(level, type, declarator_, block);
    else if (decl_list = ptr_to<Decls>(decl_or_stmt_)) {
        decl_list->decl_list.push_front(declarator_);
        return new_ptr<VarDeclList>(level, type, std::move(decl_list));
    } else
        error(R"(Conversion from type "Node" to type "Decls" failed!)");
}

ptr<Node> Parser::decl_or_stmt(int level, ptr<Type> type) {
    if (look->tag == '{') {    // declaration_list -> block
        move();
        ptr<StmtList> block_ = statement_list(level + 1);
        match('}');
        return block_;
    } else if (look->tag == ';') {// Empty declaration_list ==> decl_or_stmt -> ';'
        move();
        std::list<ptr<Decl>> decl_list_;
        return new_ptr<Decls>(level + 1, decl_list_);
    } else {    // decl_or_stmt -> ',' declaration_list ';'
        match(',');
        ptr<Decls> decl_list_ = declarator_list(level + 1, type);
        match(';');
        return decl_list_;
    }
}

ptr<Decls> Parser::declarator_list(int level, ptr<Type> type) {
    std::list<ptr<Decl>> decl_list_;
    while (look->tag == Tag::ID) {
        decl_list_.push_back(declarator(level + 1, type));
        if (look->tag == ';')
            break;
        else
            match(',');
    }
    return new_ptr<Decls>(level + 1, decl_list_);
}

ptr<Decl> Parser::declarator(int level, ptr<Type> type) {
    ptr<Id> id = new_ptr<Id>(level + 1, ptr_to<Word>(match(Tag::ID)));
    if (look->tag == '(') {
        move();
        if (look->tag == ')') {
            move();
            return new_ptr<FunctionDecl>(level + 1, id);
        } else {
            ptr<ParameterList> param_list;
            param_list = parameter_list(level + 1);
            return new_ptr<FunctionDecl>(level + 1, id, param_list);
        }
    } else if (look->tag == '[') {
        move();
        if (look->tag == ']') {     // declarator -> ID '[' ']' '=' '{' constant_list '}'
            match(']');
            match('=');
            match('{');
            ptr<ConstantList> const_list = constant_list(level + 1);
            return new_ptr<ArrayDecl>(level + 1, id, std::move(const_list));
        } else {
            ptr<Expr> expression = expr(level + 1);
            match(']');
            if (look->tag == '=') {   // declarator -> ID '[' expr ']' '=' '{' constant_list '}'
                move();
                match('{');
                ptr<ConstantList> const_list = constant_list(level + 1);
                return new_ptr<ArrayDecl>(level + 1, id, expression, const_list);
            } else {    // declarator -> ID '[' expr ']'
                return new_ptr<ArrayDecl>(level + 1, id, expression);
            }
        }
    } else if (look->tag == '=') { // declaration -> ID '=' expr
        move();
        ptr<Expr> expression = expr(level + 2);
        return new_ptr<IdDecl>(level + 1, id, expression);
    } else {    // declaration -> ID
        return new_ptr<IdDecl>(level + 1, id);
    }
}

ptr<ParameterList> Parser::parameter_list(int level) {
    std::list<ptr<Parameter>> parameter_list;
    ptr<Type> type;
    ptr<Id> id;
    while (look->tag == Tag::TYPE) {
        type = ptr_to<Type>(look);
        move();
        id = new_ptr<Id>(level, ptr_to<Word>(match(Tag::ID)));
        parameter_list.push_back(new_ptr<Parameter>(level + 1, type, id));
        if (look->tag == ',')
            move();
        else
            break;
    }
    match(')');
    return new_ptr<ParameterList>(level, parameter_list);
}

ptr<ConstantList> Parser::constant_list(int level) {
    std::list<ptr<Constant>> const_list;
    while (look->tag == Tag::NUM || look->tag == Tag::STRING) {
        if (look->tag == Tag::NUM)
            const_list.push_back(new_ptr<Constant>(level + 1, ptr_to<Num>(look)->value));
        else
            const_list.push_back(new_ptr<Constant>(level + 1, std::move(ptr_to<Word>(look)->lexeme)));
        move();
    }
    return new_ptr<ConstantList>(level, const_list);
}

ptr<StmtList> Parser::statement_list(int level) {
    std::list<ptr<Stmt>> stmt_list;
    while (look->tag != '}')
        stmt_list.push_back(statement(level + 1));
    return new_ptr<StmtList>(level, stmt_list);
}

ptr<Stmt> Parser::statement(int level) {
    switch (look->tag) {
        case Tag::TYPE :
        case Tag::VOID : {
            ptr<Type> type = ptr_to<Type>(look);
            move();
            ptr<Decls> decl_list = declarator_list(level + 2, type);
            match(';');
            return new_ptr<VarDeclList>(level + 1, type, decl_list);
        }
        case '{' : {
            move();
            auto block_ = statement_list(level + 1);
            match('}');
            return block_;
        }
        case ';' :  // Ignore expression
            move();
            return Stmt::Null;
        case '-':
        case '(':
        case Tag::ID:
        case Tag::NUM:
        case Tag::STRING: {
            auto expr__ = expr(level + 1);
            match(';');
            return new_ptr<ExprStmt>(level, expr__);
        }
        case Tag::KEYWORD: {
            ptr<Word> keyword = ptr_to<Word>(look);
            if (keyword->lexeme == "while")
                return while_stmt(level + 1);
            else if (keyword->lexeme == "return")
                return return_stmt(level + 1);
            else if (keyword->lexeme == "print")
                return print_stmt(level + 1);
            else if (keyword->lexeme == "scan")
                return scan_stmt(level + 1);
            else if (keyword->lexeme == "if") {
                ptr<Expr> expr_;
                ptr<Stmt> stmt1, stmt2;
                move();
                match('(');
                expr_ = expr(level + 2);
                match(')');
                stmt1 = statement(level + 2);
                if (look->tag == Tag::KEYWORD && ptr_to<Word>(look)->lexeme == "else") {
                    move();
                    stmt2 = statement(level + 2);
                    return new_ptr<Else>(level + 1, expr_, stmt1, stmt2);
                } else
                    return new_ptr<If>(level + 1, expr_, stmt1);
            }
        }
        default:
            error("Syntax Error!");
    }
}

ptr<While> Parser::while_stmt(int level) {
    match(Tag::KEYWORD);
    match('(');
    ptr<Expr> expr_ = expr(level + 1);
    match(')');
    ptr<Stmt> stmt = statement(level + 1);
    return new_ptr<While>(level + 1, expr_, stmt);
}

ptr<Return> Parser::return_stmt(int level) {
    match(Tag::KEYWORD);
    if (look->tag == ';') {
        move();
        return new_ptr<Return>(level);
    } else {
        ptr<Expr> ret_addr_expr = expr(level + 1);
        match(';');
        return new_ptr<Return>(level + 1, ret_addr_expr);
    }
}

ptr<Scan> Parser::scan_stmt(int level) {
    match(Tag::KEYWORD);
    auto scan_ = new_ptr<Scan>(level, id_list(level + 1));
    match(';');
    return scan_;
}

ptr<Print> Parser::print_stmt(int level) {
    ptr<Print> print_;
    match(Tag::KEYWORD);
    if (look->tag == ';') {
        move();
        return new_ptr<Print>(level);
    } else {
        print_ = new_ptr<Print>(level, expr_list(level + 1));
        match(';');
        return print_;
    }
}

ptr<IdList> Parser::id_list(int level) {
    std::list<ptr<Id>> id_list_;
    id_list_.push_back(ptr_to<Id>(match(Tag::ID)));
    while (look->tag == ',') {
        move();
        id_list_.push_back(ptr_to<Id>(match(Tag::ID)));
    }
    return new_ptr<IdList>(level, id_list_);
}

ptr<ExprList> Parser::expr_list(int level) {
    std::list<ptr<Expr>> expr_list_;
    expr_list_.push_back(expr(level + 1));
    while (look->tag == ',') {
        move();
        expr_list_.push_back(expr(level + 1));
    }
    return new_ptr<ExprList>(level, expr_list_);
}

ptr<Expr> Parser::expr(int level) {
    ptr<Expr> x = add_expr(level + 1);
    ptr<Word> cmp;
    while (look->tag == Tag::CMP) {
        cmp = ptr_to<Word>(look);
        move();
        x = new_ptr<Binary>(level, cmp, x, expr(level + 1));
    }
    return x;
}

ptr<Expr> Parser::add_expr(int level) {
    ptr<Expr> x = mul_expr(level + 1);
    while (look->tag == '+' || look->tag == '-') {
        ptr<Token> tok = look;
        move();
        x = new_ptr<Binary>(level, tok, x, mul_expr(level + 1));
    }
    return x;
}

ptr<Expr> Parser::mul_expr(int level) {
    if (look->tag == '-') { // mul_expr -> '-' primary_expr
        ptr<Token> tok = look;
        move();
        return new_ptr<Unary>(level, tok, primary_expr(level + 1));
    } else {
        ptr<Expr> x = primary_expr(level + 1);
        ptr<Word> mul_tok;
        while (look->tag == '%' || look->tag == '*' || look->tag == '*') {
            mul_tok = ptr_to<Word>(look);
            move();
            x = new_ptr<Binary>(level, mul_tok, x, primary_expr(level + 1));
        }
        return x;
    }
}

ptr<Expr> Parser::primary_expr(int level) {
    switch (look->tag) {
        case Tag::ID: {
            ptr<Id> id = new_ptr<Id>(level, ptr_to<Word>(look));
            move();
            switch (look->tag) {
                case '(': {
                    move();
                    if (look->tag == ')') {    // primary_expr -> ID '(' ')'
                        move();
                        return new_ptr<FuncCall>(level, id);
                    } else {    // primary_expr -> ID '(' expr_list ')'
                        ptr<ExprList> expr_list_ = expr_list(level + 1);
                        match(')');
                        return new_ptr<FuncCall>(level, id);
                    }
                }
                case '=': {  // primary_expr -> ID '=' expr
                    move();
                    return new_ptr<Set>(level, id, expr(level + 1));
                }
                case '[': {
                    move();
                    auto index_expr = expr(level + 1);
                    match(']');
                    ptr<Access> array_elem = new_ptr<Access>(level, id, index_expr);
                    if (look->tag == '=') { // ID '[' expr ']' = expr
                        move();
                        ptr<Expr> val_expr = expr(level + 1);
                        return new_ptr<SetElem>(level, array_elem, val_expr);
                    } else    // ID '[' expr ']'
                        return array_elem;
                }
                case Tag::ASSIGN: {  // primary_expr -> ID ASSIGN expr
                    ptr<Token> assign_tok = look;
                    move();
                    return new_ptr<Binary>(level, assign_tok, id, expr(level + 1));
                }
                default:    // primary_expr -> ID
                    return id;
            }
        }
        case Tag::NUM: {
            ptr<Num> num_tok = ptr_to<Num>(look);
            move();
            return new_ptr<Constant>(level, num_tok->value);
        }
        case Tag::STRING: {
            ptr<Word> num_tok = ptr_to<Word>(look);
            move();
            return new_ptr<Constant>(level, num_tok->lexeme);
        }
        case '(': {
            move();
            auto expr_ = expr(level + 1);
            match(')');
            return expr_;
        }
        default:
            error(("Unexpected Token with tag: " + std::to_string(look->tag)));
    }
}


