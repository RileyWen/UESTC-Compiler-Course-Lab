# About This Toy Compiler

## Structure

This compiler front end is merely a toy and only support **lexical analysis**  
and **syntax analysis** (recursive-descent parsing). The structure of
this compiler front end refers to the Java-version compiler front end in
 ***Appendix A - A Complete Front End*** of ***Dragon Book***. It does some 
 modification to adapt to the grammar below and is rewritten in C++.

## Grammar

This toy compiler follows the grammar below, 
which is slightly modified from the Lab handout for programming convenience.

```
program
    : external_declaration
    | program external_declaration
    ;

external_declaration
    : type declarator decl_or_stmt
    ;

decl_or_stmt
    : block
    | ',' declaration_list ';'
    | ';'
    :

block
    : '{' statement_list '}'
    ;

statement_list
    :
    | statement_list statement
    ;

declarator_list
    : declarator
    | declarator_list ',' declarator
    ;

declarator
    : ID
    | ID '=' expr
    | ID '[' expr ']'
    | ID '[' expr ']' '=' '{' constant_list '}'
    | ID '[' ']' '=' '{' constant_list '}'
    | ID '(' parameter_list ')'
    | ID '(' ')'
    ;

constant_list
    : constant
    | constant_list ',' constant
    ;
    
constant
    : NUMBER
    | STRING
    ;
   

parameter_list
        : parameter
        | parameter_list ',' parameter
        ;


parameter
        : type ID
        ;
type
        : INT
        | STR
        | VOID
        ;

statement
    : type declarator_list ';'
    | block
    | ';'
    | expr ';'
    | IF '(' expr ')' statement
    | IF '(' expr ')' statement ELSE statement
    | WHILE '(' expr ')' statement
    | RETURN ';'
    | RETURN expr ';'
    | PRINT ';'
    | PRINT expr_list ';'
    | SCAN id_list ';'
    ;

expr
    : add_expr
    | expr CMP add_expr
    ;

add_expr
    : mul_expr
    | add_expr '+' mul_expr
    | add_expr '-' mul_expr
    ;

mul_expr
    : primary_expr
    | mul_expr '*' primary_expr
    | mul_expr '/' primary_expr
    | mul_expr '%' primary_expr
    | '-' primary_expr
    ;

primary_expr
    : ID '(' expr_list ')'
    | ID '(' ')'
    | '(' expr ')'
    | ID
    | NUMBER
    | STRING
    | ID ASSIGN expr
    | ID '=' expr
    | ID '[' expr ']'
    | ID '[' expr ']' '=' expr
    ;

expr_list
    : expr
    | expr_list ',' expr
    ;
id_list
    : ID
    | id_list ',' ID
    ;
```