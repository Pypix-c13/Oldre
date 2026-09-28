#include "lexer.h"

typedef enum ASTNodeType {
    AST_ROOT, AST_NODE, AST_PARENT, AST_CHILDREN
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    ASTNode *next;
    ASTNode *parent;
    ASTNode **children;

    char *value;
    size_t node_count;
} ASTNode;

ASTNode *isRoot();
ASTNode *isNode(ASTNode *parent);
ASTNode *isParent(ASTNode *parent);

ASTNode *isChildren(ASTNode *parent);
void free_ast(ASTNode *buffer);