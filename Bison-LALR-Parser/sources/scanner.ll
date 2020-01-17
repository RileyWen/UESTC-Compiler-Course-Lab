%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type
  make_NUM (const std::string &s, const yy::parser::location_type& loc);
%}

id      [a-zA-Z][a-zA-Z_0-9]*
int     [0-9]+
blank   [ \t]
string  \"[^\n"]+\"

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}
{blank}+    loc.step ();
\n+         loc.lines (yyleng); loc.step ();
"//"[^\n]*  loc.step();

"="         return yy::parser::make_ASSIGN (loc);
"+="        return yy::parser::make_ADD_ASSIGN (loc);
"-="        return yy::parser::make_SUB_ASSIGN (loc);
"*="        return yy::parser::make_MUL_ASSIGN (loc);
"/="        return yy::parser::make_DIV_ASSIGN (loc);
"%="        return yy::parser::make_MOD_ASSIGN (loc);

"+"         return yy::parser::make_ADD   (loc);
"-"         return yy::parser::make_MINUS (loc);
"*"         return yy::parser::make_STAR  (loc);
"/"         return yy::parser::make_SLASH (loc);
"%"         return yy::parser::make_MOD   (loc);

"("         return yy::parser::make_LPAREN  (loc);
")"         return yy::parser::make_RPAREN  (loc);
"["         return yy::parser::make_LSQUARE (loc);
"]"         return yy::parser::make_RSQUARE (loc);
"{"         return yy::parser::make_LBRACE  (loc);
"}"         return yy::parser::make_RBRACE  (loc);

"=="        return yy::parser::make_EQ (loc);
"!="        return yy::parser::make_NE (loc);
"<"         return yy::parser::make_LT (loc);
">"         return yy::parser::make_GT (loc);

","         return yy::parser::make_COMMA     (loc);
";"         return yy::parser::make_SEMICOLON (loc);

"str"       return yy::parser::make_TYPE_STR  (loc);
"int"       return yy::parser::make_TYPE_INT  (loc);
"void"      return yy::parser::make_TYPE_VOID (loc);

"while"     return yy::parser::make_WHILE     (loc);
"return"    return yy::parser::make_RETURN    (loc);
"if"        return yy::parser::make_IF        (loc);
"else"      return yy::parser::make_ELSE      (loc);
"scan"      return yy::parser::make_SCAN      (loc);
"print"     return yy::parser::make_PRINT     (loc);

{string}    return yy::parser::make_STR(yytext, loc);
{int}       return make_NUM (yytext, loc);
{id}        return yy::parser::make_ID  (yytext, loc);
.           {
                throw yy::parser::syntax_error
                    (loc, "invalid character: " + std::string(yytext));
}
<<EOF>>     return yy::parser::make_END (loc);
%%

yy::parser::symbol_type
make_NUM (const std::string &s, const yy::parser::location_type& loc)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_NUM ((int) n, loc);
}

void
driver::scan_begin ()
{
    std::cout<<"File name in scan_begin:"<<file.c_str()<<"\n";
    yy_flex_debug = trace_scanning;
    if (file.empty () || file == "-")
        yyin = stdin;
    else if (!(yyin = fopen (file.c_str (), "r")))
    {
        std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
        exit (EXIT_FAILURE);
    }
}

void
driver::scan_end ()
{
  fclose (yyin);
}
