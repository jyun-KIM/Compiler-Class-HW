#ifndef AST_H
#define AST_H

typedef enum {
    NOD_NUM,    // 숫자
    NOD_ID,     // 변수 식별자
    NOD_BIN_OP, // 이항 연산 (+, -, *, /)
    NOD_ASSIGN, // 대입 (=)
    NOD_IF,     // IF 문
    NOD_PRINT,  // 출력
    NOD_BLOCK   // 블록 (여러 문장 연결)
} NodeType;

// 기본 노드 구조체
typedef struct ASTNode {
    NodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

// 숫자 노드
typedef struct {
    ASTNode base;
    int value;
} NumNode;

// 식별자 노드 (변수명)
typedef struct {
    ASTNode base;
    char name[32];
} IDNode;

// 이항 연산 노드 (연산자 종류 저장)
typedef struct {
    ASTNode base;
    int op; // 토큰 번호 (PLUS, MINUS 등)
} BinOpNode;

// --- 함수 선언 (이 부분이 parser.y와 맞아야 함) ---
ASTNode* new_num_node(int val);
ASTNode* new_id_node(char* name);
ASTNode* new_bin_op_node(int op_type, ASTNode* left, ASTNode* right);
ASTNode* new_assign_node(char* name, ASTNode* expr);
ASTNode* new_print_node(ASTNode* expr);
ASTNode* new_if_node(ASTNode* cond, ASTNode* stmt);
ASTNode* new_block_node(ASTNode* left, ASTNode* right); // <-- 이 부분이 없어서 에러가 났을 겁니다.

int eval(ASTNode* node);

#endif