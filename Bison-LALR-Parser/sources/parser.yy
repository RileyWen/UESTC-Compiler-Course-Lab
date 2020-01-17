%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    # include <string>
    # include "AstNode.hpp"
    class driver;
    ptr<Id> new_id(std::string name);
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "driver.hpp"
}

%define api.token.prefix {TOK_}
%token
  END           0       "end of file"
  ASSIGN        "="
  ADD_ASSIGN    "+="
  SUB_ASSIGN    "-="
  MUL_ASSIGN    "*="
  DIV_ASSIGN    "/="
  MOD_ASSIGN    "%="

  MINUS         "-"
  ADD           "+"
  STAR          "*"
  SLASH         "/"
  MOD           "%"

  LPAREN        "("
  RPAREN        ")"
  LSQUARE       "["
  RSQUARE       "]"
  LBRACE        "{"
  RBRACE        "}"

  EQ            "=="
  NE            "!="
  LT            "<"
  GT            ">"

  COMMA         ","
  SEMICOLON     ";"

  TYPE_STR      "str"
  TYPE_INT      "int"
  TYPE_VOID     "void"

  WHILE         "while"
  RETURN        "return"
  IF            "if"
  ELSE          "else"
  SCAN          "scan"
  PRINT         "print"
;


%token <std::string> ID
%token <std::string> STR
%token <int> NUM
%type < ptr<ExternDecls> >      program
%type < ptr<ExternDecls> >      external_decl_list
%type < ptr<Stmt> >             external_declaration
%type < ptr<Decls> >            declarator_list
%type < ptr<Function> >         function_definition
%type < ptr<StmtList> >         statement_list
%type < ptr<StmtList> >         block
%type < ptr<VarDeclList> >      var_declaration
%type < ptr<ConstantList> >     constant_list
%type < ptr<Decl> >             declarator
%type < ptr<ParameterList> >    parameter_list
%type < ptr<Parameter> >        parameter
%type < ptr<Stmt> >             statement
%type < ptr<Stmt> >             branch_stmt
%type < ptr<Stmt> >             expr_stmt
%type < ptr<While> >            while_stmt
%type < ptr<Return> >           return_stmt
%type < ptr<Scan> >             scan_stmt
%type < ptr<Print> >            print_stmt
%type < ptr<Expr> >             expr

%type < ptr<Expr> >             assign_expr
%type < ptr<Token> >            assign_tok

%type < ptr<Expr> >             cmp_expr
%type < ptr<Token> >            cmp_tok

%type < ptr<Expr> >             add_expr
%type < ptr<Token> >            add_tok

%type < ptr<Expr> >             mul_expr
%type < ptr<Token> >            mul_tok

%type < ptr<Expr> >             primary_expr

%type < ptr<ExprList> >         expr_list
%type < ptr<IdList> >           id_list
%type < ptr<Constant> >         constant
%type < ptr<Type> >             type


%printer { yyo << $$; } <*>;

%%
%start program;
program:
    external_decl_list { drv.result = $1; };

external_decl_list:
    external_declaration {
        std::list<ptr<Stmt>> extr_decl_list;
        extr_decl_list.push_back($1);
        $$ = new_ptr<ExternDecls>(extr_decl_list);
    }
|   external_decl_list external_declaration {
        $1->stmt_list.push_back($2);
        $$ = $1;
    };

external_declaration:
    function_definition { $$ = $1; }
|   var_declaration { $$ = $1; };

function_definition:
    type declarator block {
        $$ = new_ptr<Function>($1, ptr_to<FunctionDecl>($2), $3);
    };

var_declaration:
    type declarator_list ";" { $$ = new_ptr<VarDeclList>($1, $2); };

declarator_list:
    declarator {
        std::list<ptr<Decl>> decl_list_;
        decl_list_.push_back($1);
        $$ = new_ptr<Decls>(decl_list_);
    }
|   declarator_list "," declarator {
        $1->decl_list.push_back($3);
        $$ = $1;
    };

constant_list:
    constant {
        std::list<ptr<Constant>> const_list;
        const_list.push_back($1);
        $$ = new_ptr<ConstantList>(const_list);
    }
|   constant_list "," constant {
        $1->constlist.push_back($3);
        $$ = $1;
    };

constant:
  NUM { $$ = new_ptr<Constant>($1); }
| STR { $$ = new_ptr<Constant>($1); }
;

declarator:
    ID {
        $$ = new_ptr<IdDecl>(new_id($1));
    }
|   ID "=" expr {
        $$ = new_ptr<IdDecl>(new_id($1), $3);
    }
|   ID "(" parameter_list ")" {
        $$ = new_ptr<FunctionDecl>(new_id($1), $3);
    }
|   ID "(" ")" {
        $$ = new_ptr<FunctionDecl>(new_id($1));
    }
|   ID "[" expr "]" {
        $$ = new_ptr<ArrayDecl>(new_id($1), $3);
    }
|   ID "[" expr "]" "=" "{" constant_list "}"{
        $$ = new_ptr<ArrayDecl>(new_id($1), $3, $7);
    }
|   ID "[" "]" "=" "{" constant_list "}" {
        $$ = new_ptr<ArrayDecl>(new_id($1), $6);
    };

parameter_list:
    parameter {
        std::list<ptr<Parameter>> param_list;
        param_list.push_back($1);
        $$ = new_ptr<ParameterList>(param_list);
    }
|   parameter_list "," parameter {
        $1->parameter_list.push_back($3);
        $$ = $1;
    };

parameter:
    type ID { $$ = new_ptr<Parameter>($1, new_id($2));  };

type:
  TYPE_INT  { $$ = new_ptr<Word>("int", Tag::TYPE); }
| TYPE_STR  { $$ = new_ptr<Word>("str", Tag::TYPE); }
| TYPE_VOID { $$ = new_ptr<Word>("void", Tag::TYPE); }
;

statement:
    block { $$ = $1; }
|   expr_stmt { $$ = $1; }
|   branch_stmt { $$ = $1; }
|   while_stmt { $$ = $1; }
|   return_stmt { $$ = $1; }
|   print_stmt { $$ = $1; }
|   scan_stmt { $$ = $1; }
|   var_declaration { $$ = $1; };

block:
    "{" "}" {
        std::list<ptr<Stmt>> stmt_list;
        $$ = new_ptr<StmtList>(stmt_list);
    }
|   "{" statement_list "}" {
        $$ = $2;
    };

statement_list:
    statement {
        std::list<ptr<Stmt>> stmt_list;
        stmt_list.push_back($1);
        $$ = new_ptr<StmtList>(stmt_list);
    }
|   statement_list statement {
        $1->stmt_list.push_back($2);
        $$ = $1;
    };

expr_stmt:
    ";" { $$ = Stmt::Null; }
|   expr ";" { $$ = new_ptr<ExprStmt>($1); };

branch_stmt:
    IF "(" expr ")" statement { $$ = new_ptr<If>($3, $5); }
|   IF "(" expr ")" statement ELSE statement {
        $$ = new_ptr<Else>($3, $5, $7);
    };

while_stmt:
    WHILE "(" expr ")" statement { $$ = new_ptr<While>($3, $5); };

return_stmt:
    RETURN ";" { $$ = new_ptr<Return>(); }
|   RETURN expr ";" { $$ = new_ptr<Return>($2); };

print_stmt:
  PRINT ";" { $$ = new_ptr<Print>(); }
| PRINT expr_list ";" { $$ = new_ptr<Print>($2); };

scan_stmt:
    SCAN id_list ";" { $$ = new_ptr<Scan>($2); };

expr:
    assign_expr { $$ = $1;};

assign_expr:
    cmp_expr {
        $$ = $1;
    }
|   ID assign_tok assign_expr {
        $$ = new_ptr<Binary>($2, new_id($1), $3);
    }
|   ID "=" assign_expr {
        $$ = new_ptr<Set>(new_id($1), $3);
    }
|   ID "[" expr "]" "=" assign_expr {
        $$ = new_ptr<SetElem>(new_ptr<Access>(new_id($1), $3), $6);
    };

assign_tok:
  "*=" { $$ = new_ptr<Word>("*=", Tag::ASSIGN); }
| "/=" { $$ = new_ptr<Word>("/=", Tag::ASSIGN); }
| "+=" { $$ = new_ptr<Word>("+=", Tag::ASSIGN); }
| "-=" { $$ = new_ptr<Word>("-=", Tag::ASSIGN); }
| "%=" { $$ = new_ptr<Word>("%=", Tag::ASSIGN); }
;

cmp_expr:
    add_expr {
        $$ = $1;
  }
| cmp_expr cmp_tok add_expr {
        $$ = new_ptr<Binary>($2, $1, $3);
    };

cmp_tok:
  ">=" { $$ = new_ptr<Word>(">=", Tag::CMP); }
| "<=" { $$ = new_ptr<Word>("<=", Tag::CMP); }
| "<"  { $$ = new_ptr<Word>("<" , Tag::CMP); }
| ">"  { $$ = new_ptr<Word>(">" , Tag::CMP); }
| "==" { $$ = new_ptr<Word>("==", Tag::CMP); }
| "!=" { $$ = new_ptr<Word>("!=", Tag::CMP); }
;

add_expr:
    mul_expr {
        $$ = $1;
  }
| add_expr add_tok mul_expr {
        $$ = new_ptr<Binary>($2, $1, $3);
    };

add_tok:
    "+" { $$ = new_ptr<Token>('+'); }
|   "-" { $$ = new_ptr<Token>('-'); };



mul_expr:
    primary_expr {
        $$ = $1;
    }
|   mul_expr mul_tok  primary_expr {
        $$ = new_ptr<Binary>($2, $1, $3);
    }
|   "-" primary_expr {
        $$ = new_ptr<Unary>(new_ptr<Token>('-'), $2);
    };

mul_tok:
    "*" { $$ = new_ptr<Token>('*'); }
|   "/" { $$ = new_ptr<Token>('/'); }
|   "%" { $$ = new_ptr<Token>('%'); };


primary_expr:
    ID "(" expr_list ")" {
        $$ = new_ptr<FuncCall>(new_id($1),$3);
    }
|   ID "(" ")" {
        $$ = new_ptr<FuncCall>(new_id($1));
    }
|   "(" expr ")" {
        $$ = $2;
    }
|   ID {
        $$ = new_id($1);
    }
| constant {
        $$ = $1;
    }
| ID "[" expr "]" {
        $$ = new_ptr<Access>(new_id($1), $3);
    };

expr_list:
  expr  {
        std::list<ptr<Expr>> expr_list;
        expr_list.push_front($1);
        $$ = new_ptr<ExprList>(expr_list);
    }
| expr_list "," expr {
        $1->exprlist.push_back($3);
        $$ = $1;
};

id_list:
  ID    {
        std::list<ptr<Id>> id_list;
        id_list.push_front(new_id($1));
        $$ = new_ptr<IdList>(id_list);
    }
| id_list "," ID {
        $1->ids.push_back(new_id($3));
        $$ = $1;
};

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}

ptr<Id> new_id(std::string name){
    return new_ptr<Id>(new_ptr<Word>(name, Tag::ID));
}
