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

ptr<Stmt> Parser::program() {
    std::list<ptr<Stmt>> extr_decl_list;
    while (look->tag == Tag::TYPE || look->tag == Tag::VOID)
        extr_decl_list.push_back(external_declaration());
    if (look->tag == Tag::EOF_)
        return new_ptr<ExternDecls>(std::move(extr_decl_list));
    else
        error("Unexpected Token with Tag " + std::to_string(look->tag));
}

ptr<Stmt> Parser::external_declaration() {
    ptr<Decl> declarator_;
    ptr<Node> decl_or_stmt_;
    ptr<Type> type = ptr_to<Type>(std::move(look));
    move();
    declarator_ = declarator(type);
    decl_or_stmt_ = decl_or_stmt(type);
    ptr<Stmt> block;
    ptr<Decls> decl_list;
    if (block = ptr_to<Stmt>(decl_or_stmt_)) {
        if (auto funcdecl = ptr_to<FunctionDecl>(declarator_))
            return new_ptr<Function>(type, funcdecl, block);
        else
            error("Block must follow a function declaration");
    } else if (decl_list = ptr_to<Decls>(decl_or_stmt_)) {
        decl_list->decl_list.push_front(declarator_);
        return new_ptr<VarDeclList>(type, std::move(decl_list));
    } else
        error(R"(Conversion from type "Node" to type "Decls" failed!)");
}

ptr<Node> Parser::decl_or_stmt(ptr<Type> type) {
    if (look->tag == '{') {    // declaration_list -> block
        move();
        ptr<StmtList> block_ = statement_list();
        match('}');
        return block_;
    } else if (look->tag == ';') {// Empty declaration_list ==> decl_or_stmt -> ';'
        move();
        std::list<ptr<Decl>> decl_list_;
        return new_ptr<Decls>(decl_list_);
    } else {    // decl_or_stmt -> ',' declaration_list ';'
        match(',');
        ptr<Decls> decl_list_ = declarator_list(type);
        match(';');
        return decl_list_;
    }
}

ptr<Decls> Parser::declarator_list(ptr<Type> type) {
    std::list<ptr<Decl>> decl_list_;
    while (look->tag == Tag::ID) {
        decl_list_.push_back(declarator(type));
        if (look->tag == ';')
            break;
        else
            match(',');
    }
    return new_ptr<Decls>(decl_list_);
}

ptr<Decl> Parser::declarator(ptr<Type> type) {
    ptr<Id> id = new_ptr<Id>(ptr_to<Word>(match(Tag::ID)));
    if (look->tag == '(') {
        move();
        if (look->tag == ')') {
            move();
            return new_ptr<FunctionDecl>(id);
        } else {
            ptr<ParameterList> param_list;
            param_list = parameter_list();
            return new_ptr<FunctionDecl>(id, param_list);
        }
    } else if (look->tag == '[') {
        move();
        if (look->tag == ']') {     // declarator -> ID '[' ']' '=' '{' constant_list '}'
            match(']');
            match('=');
            match('{');
            ptr<ConstantList> const_list = constant_list();
            return new_ptr<ArrayDecl>(id, std::move(const_list));
        } else {
            ptr<Expr> expression = expr();
            match(']');
            if (look->tag == '=') {   // declarator -> ID '[' expr ']' '=' '{' constant_list '}'
                move();
                match('{');
                ptr<ConstantList> const_list = constant_list();
                return new_ptr<ArrayDecl>(id, expression, const_list);
            } else {    // declarator -> ID '[' expr ']'
                return new_ptr<ArrayDecl>(id, expression);
            }
        }
    } else if (look->tag == '=') { // declaration -> ID '=' expr
        move();
        ptr<Expr> expression = expr();
        return new_ptr<IdDecl>(id, expression);
    } else {    // declaration -> ID
        return new_ptr<IdDecl>(id);
    }
}

ptr<ParameterList> Parser::parameter_list() {
    std::list<ptr<Parameter>> parameter_list;
    ptr<Type> type;
    ptr<Id> id;
    while (look->tag == Tag::TYPE) {
        type = ptr_to<Type>(look);
        move();
        id = new_ptr<Id>(ptr_to<Word>(match(Tag::ID)));
        parameter_list.push_back(new_ptr<Parameter>(type, id));
        if (look->tag == ',')
            move();
        else
            break;
    }
    match(')');
    return new_ptr<ParameterList>(parameter_list);
}

ptr<ConstantList> Parser::constant_list() {
    std::list<ptr<Constant>> const_list;
    while (look->tag == Tag::NUM || look->tag == Tag::STRING) {
        if (look->tag == Tag::NUM)
            const_list.push_back(new_ptr<Constant>(ptr_to<Num>(look)->value));
        else
            const_list.push_back(new_ptr<Constant>(std::move(ptr_to<Word>(look)->lexeme)));
        move();
    }
    return new_ptr<ConstantList>(const_list);
}

ptr<StmtList> Parser::statement_list() {
    std::list<ptr<Stmt>> stmt_list;
    while (look->tag != '}')
        stmt_list.push_back(statement());
    return new_ptr<StmtList>(stmt_list);
}

ptr<Stmt> Parser::statement() {
    switch (look->tag) {
        case Tag::TYPE :
        case Tag::VOID : {
            ptr<Type> type = ptr_to<Type>(look);
            move();
            ptr<Decls> decl_list = declarator_list(type);
            match(';');
            return new_ptr<VarDeclList>(type, decl_list);
        }
        case '{' : {
            move();
            auto block_ = statement_list();
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
            auto expr__ = expr();
            match(';');
            return new_ptr<ExprStmt>(expr__);
        }
        case Tag::KEYWORD: {
            ptr<Word> keyword = ptr_to<Word>(look);
            if (keyword->lexeme == "while")
                return while_stmt();
            else if (keyword->lexeme == "return")
                return return_stmt();
            else if (keyword->lexeme == "print")
                return print_stmt();
            else if (keyword->lexeme == "scan")
                return scan_stmt();
            else if (keyword->lexeme == "if") {
                ptr<Expr> expr_;
                ptr<Stmt> stmt1, stmt2;
                move();
                match('(');
                expr_ = expr();
                match(')');
                stmt1 = statement();
                if (look->tag == Tag::KEYWORD && ptr_to<Word>(look)->lexeme == "else") {
                    move();
                    stmt2 = statement();
                    return new_ptr<Else>(expr_, stmt1, stmt2);
                } else
                    return new_ptr<If>(expr_, stmt1);
            }
        }
        default:
            error("Syntax Error!");
    }
}

ptr<While> Parser::while_stmt() {
    match(Tag::KEYWORD);
    match('(');
    ptr<Expr> expr_ = expr();
    match(')');
    ptr<Stmt> stmt = statement();
    return new_ptr<While>(expr_, stmt);
}

ptr<Return> Parser::return_stmt() {
    match(Tag::KEYWORD);
    if (look->tag == ';') {
        move();
        return new_ptr<Return>();
    } else {
        ptr<Expr> ret_addr_expr = expr();
        match(';');
        return new_ptr<Return>(ret_addr_expr);
    }
}

ptr<Scan> Parser::scan_stmt() {
    match(Tag::KEYWORD);
    auto scan_ = new_ptr<Scan>(id_list());
    match(';');
    return scan_;
}

ptr<Print> Parser::print_stmt() {
    ptr<Print> print_;
    match(Tag::KEYWORD);
    if (look->tag == ';') {
        move();
        return new_ptr<Print>();
    } else {
        print_ = new_ptr<Print>(expr_list());
        match(';');
        return print_;
    }
}

ptr<IdList> Parser::id_list() {
    std::list<ptr<Id>> id_list_;
    id_list_.push_back(ptr_to<Id>(match(Tag::ID)));
    while (look->tag == ',') {
        move();
        id_list_.push_back(ptr_to<Id>(match(Tag::ID)));
    }
    return new_ptr<IdList>(id_list_);
}

ptr<ExprList> Parser::expr_list() {
    std::list<ptr<Expr>> expr_list_;
    expr_list_.push_back(expr());
    while (look->tag == ',') {
        move();
        expr_list_.push_back(expr());
    }
    return new_ptr<ExprList>(expr_list_);
}

ptr<Expr> Parser::expr() {
    ptr<Expr> x = add_expr();
    ptr<Word> cmp;
    while (look->tag == Tag::CMP) {
        cmp = ptr_to<Word>(look);
        move();
        x = new_ptr<Binary>(cmp, x, expr());
    }
    return x;
}

ptr<Expr> Parser::add_expr() {
    ptr<Expr> x = mul_expr();
    while (look->tag == '+' || look->tag == '-') {
        ptr<Token> tok = look;
        move();
        x = new_ptr<Binary>(tok, x, mul_expr());
    }
    return x;
}

ptr<Expr> Parser::mul_expr() {
    if (look->tag == '-') { // mul_expr -> '-' primary_expr
        ptr<Token> tok = look;
        move();
        return new_ptr<Unary>(tok, primary_expr());
    } else {
        ptr<Expr> x = primary_expr();
        while (look->tag == '%' || look->tag == '*' || look->tag == '*') {
            ptr<Token> tok = look;
            move();
            x = new_ptr<Binary>(tok, x, primary_expr());
        }
        return x;
    }
}

ptr<Expr> Parser::primary_expr() {
    switch (look->tag) {
        case Tag::ID: {
            ptr<Id> id = new_ptr<Id>(ptr_to<Word>(look));
            move();
            switch (look->tag) {
                case '(': {
                    move();
                    if (look->tag == ')') {    // primary_expr -> ID '(' ')'
                        move();
                        return new_ptr<FuncCall>(id);
                    } else {    // primary_expr -> ID '(' expr_list ')'
                        ptr<ExprList> expr_list_ = expr_list();
                        match(')');
                        return new_ptr<FuncCall>(id, expr_list_);
                    }
                }
                case '=': {  // primary_expr -> ID '=' expr
                    move();
                    return new_ptr<Set>(id, expr());
                }
                case '[': {
                    move();
                    auto index_expr = expr();
                    match(']');
                    ptr<Access> array_elem = new_ptr<Access>(id, index_expr);
                    if (look->tag == '=') { // ID '[' expr ']' = expr
                        move();
                        ptr<Expr> val_expr = expr();
                        return new_ptr<SetElem>(array_elem, val_expr);
                    } else    // ID '[' expr ']'
                        return array_elem;
                }
                case Tag::ASSIGN: {  // primary_expr -> ID ASSIGN expr
                    ptr<Token> assign_tok = look;
                    move();
                    return new_ptr<Binary>(assign_tok, id, expr());
                }
                default:    // primary_expr -> ID
                    return id;
            }
        }
        case Tag::NUM: {
            ptr<Num> num_tok = ptr_to<Num>(look);
            move();
            return new_ptr<Constant>(num_tok->value);
        }
        case Tag::STRING: {
            ptr<Word> num_tok = ptr_to<Word>(look);
            move();
            return new_ptr<Constant>(num_tok->lexeme);
        }
        case '(': {
            move();
            auto expr_ = expr();
            match(')');
            return expr_;
        }
        default:
            error(("Unexpected Token with tag: " + std::to_string(look->tag)));
    }
}


