%{
	int line_num = 1;
	char string_buf[8192];
        char *string_buf_ptr;
%}

%option noyywrap

%x single_quote_str
%x double_quote_str
%x single_comment
%x double_comment

Digit	[0-9]
Digits	{Digit}+

%%

("+"|"-")?{Digits}		{ printf ("<Int,\t\t \"%s\">\n", yytext); }
("+"|"-")?{Digits}"."{Digits}	{ printf ("<Float,\t\t\"%s\">\n", yytext); }

 /****************************************************/
\"      string_buf_ptr = string_buf; BEGIN(double_quote_str);

<double_quote_str>\"        {
        BEGIN(INITIAL);
        *string_buf_ptr = '\0';
		printf ("<String,\t \"%s\">\n", string_buf);
        }

<double_quote_str>[^\\\n\"]+        {
        char *yptr = yytext;
        while ( *yptr )
                *string_buf_ptr++ = *yptr++;
        }

 /****************************************************/
\'      string_buf_ptr = string_buf; BEGIN(single_quote_str);

<single_quote_str>\'        {
        BEGIN(INITIAL);
        *string_buf_ptr = '\0';
		printf ("<String,\t \"%s\">\n", string_buf);
        }

<single_quote_str>[^\\\n\']+        {
        char *yptr = yytext;
        while ( *yptr )
                *string_buf_ptr++ = *yptr++;
        }

 /****************************************************/

"#"[^\n]* ;

 /****************************************************/

\"\"\"       BEGIN(double_comment);

<double_comment>"\"\"\""       		{ BEGIN(INITIAL); printf("!");}
<double_comment>[^\n"]*        		;/*printf("%s",yytext); */
<double_comment>"\""{1,2}[^\"]*   	;
<double_comment>\n           		line_num++;

 /****************************************************/

"\'\'\'"       BEGIN(single_comment);

<single_comment>"\'\'\'"            {BEGIN(INITIAL);}
<single_comment>[^\n']*        		; /*printf("%s",yytext);*/
<single_comment>"\'"{1,2}[^\']*   	;
<single_comment>\n             		line_num++;



[\t ]+					/* ignore whitespace */ ;
\n						{ ++line_num; printf("<EOL>\n"); }

and		    |
as		    |
assert		|
break		|
class		|
continue	|
def		    |
del		    |
elif		|
else		|
except		|
exec		|
finally		|
for		    |
from		|
global		|
if		    |
import		|
in			|
is			|
lambda		|
not			|
or			|
pass		|
print		|
raise		|
return		|
try			|
while		|
with		|
yield					{ printf ("<Keyword,\t \"%s\">\n", yytext); }

[A-Za-z][[:alnum:]_]*	{ printf ("<Identifer,\t \"%s\">\n", yytext); }



"="						{ printf("<Assign,\t \"%s\">\n", yytext);}


"!="	|
">="	|
"<="	|
"<"		|
">"		{ printf("<Rel_OP,\t \"%s\">\n", yytext); }

"+"		|
"-"		|
"*"		|
"/"		{ printf("<Arith_OP,\t \"%s\">\n", yytext); }

\.		{ printf("<Access,\t \"%s\">\n", yytext); }

.		{ printf("<\"%s\">\n", yytext); }
%%

int main(int argc, char **argv)
{
	yyin = fopen( "../test-input/python.py", "r" );
	yylex();
}












