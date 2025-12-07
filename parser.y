%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yylineno;
void yyerror(const char* s);
ASTNode* root; 
%}

%error-verbose

%union {
    int ival;
    char sval[32];
    struct ASTNode* node;
}

%token <ival> NUMBER
%token <sval> IDENTIFIER
%token TOK_PRINT TOK_IF TOK_WHILE TOK_SCAN TOK_LET TOK_PIPE
%token PLUS MINUS MULT DIV ASSIGN EQ LT GT
%token LPAREN RPAREN LBRACE RBRACE SEMI

%type <node> program stmt_list stmt expr 

%left TOK_PIPE 
%left EQ LT GT
%left PLUS MINUS
%left MULT DIV

%%

program:
    stmt_list { root = $1; }
    ;

stmt_list:
    stmt { $$ = $1; }
    | stmt_list stmt { 
        $$ = new_block_node($1, $2); 
    }
    ;

stmt:
    TOK_LET IDENTIFIER ASSIGN expr SEMI { 
        $$ = new_assign_node($2, $4);
    }
    | IDENTIFIER ASSIGN expr SEMI {
        $$ = new_assign_node($1, $3);
    }
    | TOK_PRINT LPAREN expr RPAREN SEMI {
        $$ = new_print_node($3);
    }
    | expr TOK_PIPE TOK_PRINT SEMI {
        $$ = new_print_node($1);
    }
    | TOK_IF LPAREN expr RPAREN LBRACE stmt_list RBRACE {
        $$ = new_if_node($3, $6);
    }
    /* [NEW] While 문법 */
    | TOK_WHILE LPAREN expr RPAREN LBRACE stmt_list RBRACE {
        $$ = new_while_node($3, $6);
    }
    ;

expr:
    expr PLUS expr { $$ = new_bin_op_node(PLUS, $1, $3); }
    | expr MINUS expr { $$ = new_bin_op_node(MINUS, $1, $3); }
    | expr MULT expr { $$ = new_bin_op_node(MULT, $1, $3); }
    | expr DIV expr { $$ = new_bin_op_node(DIV, $1, $3); }
    | expr EQ expr { $$ = new_bin_op_node(EQ, $1, $3); }
    | expr LT expr { $$ = new_bin_op_node(LT, $1, $3); }
    | expr GT expr { $$ = new_bin_op_node(GT, $1, $3); }
    | LPAREN expr RPAREN { $$ = $2; }
    /* [NEW] Scan 문법 (함수 호출처럼 사용) */
    | TOK_SCAN LPAREN RPAREN { $$ = new_scan_node(); }
    | NUMBER { $$ = new_num_node($1); }
    | IDENTIFIER { $$ = new_id_node($1); }
    ;

%%

void yyerror(const char* s) {
    extern int yylineno; 
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}