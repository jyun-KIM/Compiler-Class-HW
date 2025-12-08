#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h" 

// Flex의 전역 변수들
extern int yyparse();
extern FILE* yyin; 

// 심볼 테이블
struct Symbol { char name[32]; int val; };
struct Symbol sym_table[100];
int sym_count = 0;
int is_game_mode = 0;

int get_val(char* name) {
    for(int i=0; i<sym_count; i++) {
        if(strcmp(sym_table[i].name, name) == 0) return sym_table[i].val;
    }
    printf("Runtime Error: Undefined variable '%s'\n", name);
    exit(1);
}

void set_val(char* name, int val) {
    for(int i=0; i<sym_count; i++) {
        if(strcmp(sym_table[i].name, name) == 0) {
            sym_table[i].val = val;
            return;
        }
    }
    strcpy(sym_table[sym_count].name, name);
    sym_table[sym_count].val = val;
    sym_count++;
}

// --- 노드 생성 함수들 ---

ASTNode* new_num_node(int val) {
    NumNode* node = malloc(sizeof(NumNode));
    node->base.type = NOD_NUM;
    node->base.left = NULL; node->base.right = NULL;
    node->value = val;
    return (ASTNode*)node;
}

ASTNode* new_id_node(char* name) {
    IDNode* node = malloc(sizeof(IDNode));
    node->base.type = NOD_ID;
    node->base.left = NULL; node->base.right = NULL;
    strcpy(node->name, name);
    return (ASTNode*)node;
}

ASTNode* new_bin_op_node(int op_type, ASTNode* left, ASTNode* right) {
    BinOpNode* node = malloc(sizeof(BinOpNode));
    node->base.type = NOD_BIN_OP;
    node->base.left = left;
    node->base.right = right;
    node->op = op_type;
    return (ASTNode*)node;
}

ASTNode* new_assign_node(char* name, ASTNode* expr) {
    IDNode* node = malloc(sizeof(IDNode));
    node->base.type = NOD_ASSIGN;
    node->base.left = NULL; 
    node->base.right = expr;
    strcpy(node->name, name);
    return (ASTNode*)node;
}

ASTNode* new_print_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NOD_PRINT;
    node->left = expr;
    node->right = NULL;
    return node;
}

ASTNode* new_if_node(ASTNode* cond, ASTNode* stmt) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NOD_IF;
    node->left = cond;
    node->right = stmt;
    return node;
}

ASTNode* new_while_node(ASTNode* cond, ASTNode* stmt) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NOD_WHILE;
    node->left = cond;
    node->right = stmt;
    return node;
}

ASTNode* new_scan_node() {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NOD_SCAN;
    node->left = NULL; node->right = NULL;
    return node;
}

ASTNode* new_block_node(ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NOD_BLOCK;
    node->left = left;
    node->right = right;
    return node;
}

// --- AST 시각화 ---

void generate_dot(ASTNode* node, FILE* fp) {
    if (!node) return;
    fprintf(fp, "  node%p [label=\"", node);
    switch(node->type) {
        case NOD_NUM: fprintf(fp, "%d", ((NumNode*)node)->value); break;
        case NOD_ID: fprintf(fp, "Var\\n%s", ((IDNode*)node)->name); break;
        case NOD_BIN_OP: {
             int op = ((BinOpNode*)node)->op;
             if      (op == PLUS)  fprintf(fp, "+");
             else if (op == MINUS) fprintf(fp, "-");
             else if (op == MULT)  fprintf(fp, "*");
             else if (op == DIV)   fprintf(fp, "/");
             else if (op == EQ)    fprintf(fp, "==");
             else if (op == LT)    fprintf(fp, "<");
             else if (op == GT)    fprintf(fp, ">");
             else                  fprintf(fp, "Op(%d)", op);
             break;
        }
        case NOD_ASSIGN: fprintf(fp, "Assign\\n(%s)", ((IDNode*)node)->name); break;
        case NOD_PRINT: fprintf(fp, "Print"); break;
        case NOD_IF: fprintf(fp, "IF"); break;
        case NOD_WHILE: fprintf(fp, "WHILE"); break;
        case NOD_SCAN: fprintf(fp, "SCAN"); break;
        case NOD_BLOCK: fprintf(fp, "Block"); break;
        default: fprintf(fp, "Unknown");
    }
    fprintf(fp, "\"];\n");
    if (node->left) {
        fprintf(fp, "  node%p -> node%p;\n", node, node->left);
        generate_dot(node->left, fp);
    }
    if (node->right) {
        fprintf(fp, "  node%p -> node%p;\n", node, node->right);
        generate_dot(node->right, fp);
    }
}

// --- 인터프리터 실행 ---

int eval(ASTNode* node) {
    if (!node) return 0;
    switch (node->type) {
        case NOD_NUM: return ((NumNode*)node)->value;
        case NOD_ID: return get_val(((IDNode*)node)->name);
        case NOD_ASSIGN: {
            int val = eval(node->right);
            set_val(((IDNode*)node)->name, val);
            return val;
        }
        case NOD_BIN_OP: {
            int lhs = eval(node->left);
            int rhs = eval(node->right);
            int op = ((BinOpNode*)node)->op;
            if (op == PLUS) return lhs + rhs;
            if (op == MINUS) return lhs - rhs;
            if (op == MULT) return lhs * rhs;
            if (op == DIV) {
                if(rhs == 0) { printf("Runtime Error: Division by Zero\n"); exit(1); }
                return lhs / rhs;
            }
            if (op == EQ) return lhs == rhs;
            if (op == LT) return lhs < rhs;
            if (op == GT) return lhs > rhs;
            return 0;
        }
        case NOD_PRINT: {
            int val = eval(node->left);
            if (is_game_mode) {
                if (val == 900) {
                    printf("\n======================================\n");
                    printf("      🎮  UP & DOWN GAME  🎮\n");
                    printf("     (Guess Number: 0 ~ 100)\n");
                    printf("======================================\n");
                } else if (val == 800) {
                    printf("👀 [CHEAT MODE] Answer is: "); 
                } else if (val == 1) {
                    printf("   ▲ UP! (더 큰 수입니다)\n");
                } else if (val == 2) {
                    printf("   ▼ DOWN! (더 작은 수입니다)\n");
                } else if (val == 99) {
                    printf("   ⚠️  ERROR: 0~100 사이의 숫자를 입력하세요! ⚠️\n");
                } else if (val == 7777) {
                    printf("\n   🎉 CORRECT! 정답입니다! 🎉\n");
                    printf("======================================\n\n");
                }else {
                    printf("%d\n", val);
                }
            } else{
                printf("%d\n", val);
            }
        }
        case NOD_BLOCK:
            eval(node->left);
            eval(node->right);
            return 0;
        case NOD_IF:
            if (eval(node->left)) eval(node->right);
            return 0;
        case NOD_WHILE:
            while (eval(node->left)) eval(node->right);
            return 0;
        case NOD_SCAN: {
            int input_val;
            printf("   👉 Input Number: ");
            if (scanf("%d", &input_val) != 1) {
                printf("Runtime Error: Invalid Input\n");
                exit(1);
            }
            return input_val;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    extern ASTNode* root;

    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("   🚀 My Language Interpreter v1.0 🚀\n");
        printf("========================================\n");
        printf("  1. 🧮 Simple Calculator\n");
        printf("  2. 🎮 Up & Down Game\n");
        printf("  3. ❌ Exit\n"); 
        printf("========================================\n");
        printf("Select Menu >> ");
        scanf("%d", &choice);

        // 입력 버퍼 비우기
        while (getchar() != '\n');

        if (choice == 1) {
            printf("\n>>> [Calculator Mode] Enter formulas (e.g. 3+5;). Press Ctrl+D to finish.\n");
            yyin = stdin; 
            is_game_mode = 0;
            break; // 메뉴 루프 탈출 -> 실행
        }
        else if (choice == 2) {
            printf("\n>>> Loading 'game.mc'...\n");
            yyin = fopen("game.mc", "r");
            is_game_mode = 1;
            if (!yyin) {
                printf("Error: 'game.mc' file not found!\n");
                continue; 
            }
            break; // 메뉴 루프 탈출 -> 실행
        }
        else if (choice == 3) { // 3번 선택 시 종료
            printf("Good Bye!\n");
            return 0;
        }
        else {
            printf("Invalid selection!\n");
        }
    }
    

    // 선택된 모드로 실행
    if (yyparse() == 0) {
        printf("\n"); // 결과 출력 전 줄바꿈
        
        FILE* fp = fopen("ast.dot", "w");
        if(fp) {
            fprintf(fp, "digraph AST {\n");
            fprintf(fp, "  node [shape=box, fontname=\"Arial\"];\n");
            generate_dot(root, fp);
            fprintf(fp, "}\n");
            fclose(fp);
        }

        eval(root); 
    }
    
    return 0;
}