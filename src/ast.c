#include "include/ast.h"
#include <stdio.h>

ASTNode *isRoot() {
    ASTNode *root = malloc(sizeof(ASTNode));
    root->type = AST_ROOT;
    root->value = null;
    root->next = null;
    root->parent = null;
    root->children = null;
    root->node_count = 0;
    return root;
}

ASTNode *isNode(ASTNode *parent) {
    ASTNode *node = isRoot();
    node->type = AST_NODE;
    node->parent = parent;

    if(parent) {
        ASTNode **new_children = realloc(
            parent->children,
            sizeof(ASTNode*) * (parent->node_count + 1)
        );

        if(!new_children) {
            free(node);
            return null;
        }

        parent->children = new_children;
        parent->children[parent->node_count] = node;
        parent->node_count++;
    }

    return node;
}

ASTNode *isParent(ASTNode *parent) {
    ASTNode *parent = isNode(parent);
    parent->type = AST_PARENT;
    return parent;
}

ASTNode *isChildren(ASTNode *parent) {
    ASTNode *child = isNode(parent);
    child->type = AST_CHILDREN;
    return child;
}

void free_ast(ASTNode *buffer) {
    if(!buffer) return;
    for(size_t i = 0;i < buffer->node_count;i++) {
        free_ast(buffer->children[i]);
    }

    free(buffer->value);
    free(buffer->children);
    free(buffer);
}

void expect(Token *current_token, TokenType type, Lexer *lexer, const char *error_msg) {
    if(current_token->type == type) {
        Token *t = tokenize(lexer);
        *current_token = *t;
        free(t);
    } else {
        fprintf(stderr, "[Syntax Error %zu:%zu] %s\n", current_token->line,
            current_token->column, error_msg);
        exit(1);
    }
}

ASTNode *IntStruct(Token *token, Lexer *lexer) {
    expect(token, TYPE_INT, lexer, "expect 'int' keyword!");
}