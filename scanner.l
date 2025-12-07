%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
%}

%option yylineno

%%

"print"     { return PRINT; }
"if"        { return IF; }
"let"       { return LET; }
"|>"        { return PIPE; } /* 파이프 연산자 */

[0-9]+      { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { strncpy(yylval.sval, yytext, 31); return IDENTIFIER; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"="         { return ASSIGN; }
"=="        { return EQ; }
"<"         { return LT; }
">"         { return GT; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
";"         { return SEMI; }

[ \t\n]     ; /* 공백 무시 */
.           { printf("Unknown token: %s\n", yytext); }

%%

int yywrap() { return 1; }