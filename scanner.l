%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
%}

%option yylineno

%%

"//".* ; /* 주석 처리: // 뒤에 오는 건 무시 */

"print"     { return TOK_PRINT; }
"if"        { return TOK_IF; }
"while"     { return TOK_WHILE; } /* NEW */
"scan"      { return TOK_SCAN; }  /* NEW */
"let"       { return TOK_LET; }
"|>"        { return TOK_PIPE; }

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

[ \t\n]     ;
.           { printf("Unknown token: %s\n", yytext); }

%%

int yywrap() { return 1; }