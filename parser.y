%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
void yyerror(const char* s);
ASTNode* root; // 전역 AST 루트
%}

/* Mac 기본 Bison 호환성을 위한 옵션 */
%error-verbose

%union {
    int ival;
    char sval[32];
    struct ASTNode* node;
}

%token <ival> NUMBER
%token <sval> IDENTIFIER
%token PRINT IF LET PIPE
%token PLUS MINUS MULT DIV ASSIGN EQ LT GT
%token LPAREN RPAREN LBRACE RBRACE SEMI

/* 타입 지정 */
%type <node> program stmt_list stmt expr 

/* 연산자 우선순위 */
%left PIPE 
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
    LET IDENTIFIER ASSIGN expr SEMI { 
        $$ = new_assign_node($2, $4);
    }
    | IDENTIFIER ASSIGN expr SEMI {
        $$ = new_assign_node($1, $3);
    }
    | PRINT LPAREN expr RPAREN SEMI {
        $$ = new_print_node($3);
    }
    | expr PIPE PRINT SEMI {  /* 파이프 문법: 10 |> print; */
        $$ = new_print_node($1);
    }
    | IF LPAREN expr RPAREN LBRACE stmt_list RBRACE {
        $$ = new_if_node($3, $6);
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
    | NUMBER { $$ = new_num_node($1); }
    | IDENTIFIER { $$ = new_id_node($1); }
    ;

%%

void yyerror(const char* s) {
    // 핵심 수정 사항: 이 변수가 외부에 있음을 알림
    extern int yylineno; 
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}