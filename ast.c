#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h" // PLUS, MINUS, EQ, LT, GT 등 토큰 상수

// 심볼 테이블
struct Symbol { char name[32]; int val; };
struct Symbol sym_table[100];
int sym_count = 0;

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
    node->base.left = NULL; // 대입문의 왼쪽(변수명)은 노드 내부에 저장됨
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

ASTNode* new_block_node(ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NOD_BLOCK;
    node->left = left;
    node->right = right;
    return node;
}

// --- AST 시각화 (Graphviz DOT 생성) --- 

void generate_dot(ASTNode* node, FILE* fp) {
    if (!node) return;
    
    // 현재 노드 그리기 (메모리 주소를 ID로 사용)
    fprintf(fp, "  node%p [label=\"", node);

    switch(node->type) {
        case NOD_NUM: 
            fprintf(fp, "%d", ((NumNode*)node)->value); 
            break;
            
        case NOD_ID: 
            fprintf(fp, "Var\\n%s", ((IDNode*)node)->name); 
            break;
            
        case NOD_BIN_OP: {
             int op = ((BinOpNode*)node)->op;
             if      (op == PLUS)  fprintf(fp, "+");
             else if (op == MINUS) fprintf(fp, "-");
             else if (op == MULT)  fprintf(fp, "*");
             else if (op == DIV)   fprintf(fp, "/");
             else if (op == EQ)    fprintf(fp, "==");
             else if (op == LT)    fprintf(fp, "<");
             else if (op == GT)    fprintf(fp, ">");
             // 파이프 연산자 등이 있다면 여기에 추가
             // else if (op == PIPE)  fprintf(fp, "|>");
             else                  fprintf(fp, "Op(%d)", op);
             break;
        }

        case NOD_ASSIGN: 
            // 대입 노드는 자식이 없고 내부에 이름을 가짐. 따라서 라벨에 표시
            fprintf(fp, "Assign\\n(%s)", ((IDNode*)node)->name); 
            break;
            
        case NOD_PRINT: 
            fprintf(fp, "Print"); 
            break;
            
        case NOD_IF: 
            fprintf(fp, "IF"); 
            break;
            
        case NOD_BLOCK: 
            fprintf(fp, "Block"); 
            break;
            
        default: 
            fprintf(fp, "Unknown");
    }
    fprintf(fp, "\"];\n");

    // 자식 노드 연결선 그리기 (재귀 호출)
    if (node->left) {
        fprintf(fp, "  node%p -> node%p [label=\"L\"];\n", node, node->left);
        generate_dot(node->left, fp);
    }
    if (node->right) {
        fprintf(fp, "  node%p -> node%p [label=\"R\"];\n", node, node->right);
        generate_dot(node->right, fp);
    }
}

// --- 인터프리터 실행 (Eval) ---

int eval(ASTNode* node) {
    if (!node) return 0;

    switch (node->type) {
        case NOD_NUM:
            return ((NumNode*)node)->value;
            
        case NOD_ID:
            return get_val(((IDNode*)node)->name);

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
            printf("%d\n", val);
            return val;
        }

        case NOD_BLOCK:
            eval(node->left);
            eval(node->right);
            return 0;
            
        case NOD_IF:
            if (eval(node->left)) // 조건이 참이면
                eval(node->right); // 실행
            return 0;
    }
    return 0;
}

int main() {
    extern ASTNode* root;
    extern int yyparse();
    
    printf(">>> Interpreter Start (Type code and press Ctrl+D to run)\n");
    
    if (yyparse() == 0) {
        printf(">>> Syntax OK. Executing...\n");
        eval(root); // 실행

        // --- AST 파일 생성 로직 추가 ---
        FILE* fp = fopen("ast.dot", "w");
        if (fp) {
            fprintf(fp, "digraph AST {\n");
            fprintf(fp, "  node [shape=box, fontname=\"Arial\"];\n"); // 노드 스타일 설정
            generate_dot(root, fp);
            fprintf(fp, "}\n");
            fclose(fp);
            printf(">>> AST generated: ast.dot\n");
            printf(">>> (Use 'dot -Tpng ast.dot -o ast.png' to visualize)\n");
        }
    }
    return 0;
}