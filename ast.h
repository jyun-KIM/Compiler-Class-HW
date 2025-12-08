#ifndef AST_H
#define AST_H

typedef enum {
    NOD_NUM,    // 숫자
    NOD_ID,     // 변수
    NOD_BIN_OP, // 이항 연산
    NOD_ASSIGN, // 대입
    NOD_IF,     // IF 문
    NOD_WHILE,  // WHILE 문
    NOD_SCAN,   // 입력 함수 
    NOD_PRINT,  // 출력
    NOD_BLOCK   // 블록
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

typedef struct {
    ASTNode base;
    int value;
} NumNode;

typedef struct {
    ASTNode base;
    char name[32];
} IDNode;

typedef struct {
    ASTNode base;
    int op; 
} BinOpNode;

// 함수 선언
ASTNode* new_num_node(int val);
ASTNode* new_id_node(char* name);
ASTNode* new_bin_op_node(int op_type, ASTNode* left, ASTNode* right);
ASTNode* new_assign_node(char* name, ASTNode* expr);
ASTNode* new_print_node(ASTNode* expr);
ASTNode* new_if_node(ASTNode* cond, ASTNode* stmt);
ASTNode* new_while_node(ASTNode* cond, ASTNode* stmt); // NEW
ASTNode* new_scan_node();                              // NEW
ASTNode* new_block_node(ASTNode* left, ASTNode* right);

int eval(ASTNode* node);
void generate_dot(ASTNode* node, FILE* fp);

#endif