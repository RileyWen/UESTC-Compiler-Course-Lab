%option noyywrap
%{
    enum yytokentype{
        TYPE = 257,
        EOL = 258,
        STRING  = 259,
        KEYWORD = 260,
        REL = 261,
        ID = 262,
        FLOAT = 263,
        ASSIGN = 264,
        INT = 265,
        UNKNOWN = 266,
        VOID = 267,
        OP = 268
    };
	char string_buf[8192];
    char *string_buf_ptr;
    union {
        int intval;
        float floatval;
        char* strptr;
    } yylval;
%}


%x double_quote_str

Digit	[0-9]
Digits	{Digit}+

%%

{Digits}		{ yylval.intval=atoi(yytext);  return INT; }
{Digits}"."{Digits}	{ yylval.floatval=atof(yytext); return FLOAT; }

 /****************************************************/
\"      string_buf_ptr = string_buf; BEGIN(double_quote_str);

<double_quote_str>\"        {
        BEGIN(INITIAL);
        *string_buf_ptr = '\0';
		yylval.strptr = string_buf;
        return STRING;
        }

<double_quote_str>[^\n\"]+        {
        char *yptr = yytext;
        while ( *yptr )
                *string_buf_ptr++ = *yptr++;
        }


[\t ]+					/* ignore whitespace */ ;
\n						{ return EOL; }

"/""/"[^\n]*  {}

 /*keyword*/
print	|
return  |
if      |
else    |
while   { yylval.strptr = yytext; return KEYWORD; }

 /*type*/
str     |
int     { yylval.strptr = yytext; return TYPE; }
void    { yylval.strptr = yytext; return VOID; }

 /*identifier*/
[A-Za-z][[:alnum:]_]*	{ yylval.strptr = yytext; return ID; }

"*="    |
"/="    |
"+="    |
"-="    |
"%="    { yylval.strptr = yytext; return ASSIGN;}

"<"		|
">"     |
\=\=    |
\!\=    |
\>\=    |
\<\=    { yylval.strptr = yytext; return REL; }

"-"		|
"%"     |
"+"		|
"*"		|
"/"     |
"("     |
")"     |
";"     |
"["     |
"]"     |
"{"     |
"}"     |
","     |
"="		{ return *yytext; }

.		{ yylval.strptr = yytext; return UNKNOWN; } /* Unknown Character*/
%%
