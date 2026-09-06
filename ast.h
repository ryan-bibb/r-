#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum
{
    NODE_NUMBER,
    NODE_BINOP,
    NODE_PRINT,
} NodeType;

typedef struct Node
{
    NodeType type;
    int value;           // used when type == NODE_NUMBER
    char operation;      // used when type == NODE_BINOP
    struct Node *left;   // used when type == NODE_BINOP -- 
    struct Node *right;  // used when type == NODE_BINOP -- 
} Node;

extern int parser_position;

Token *peek(void);
Token *expect(TokenType type);

Node *parse_statement(void);
Node *parse_expr(void);
Node *parse_term(void);
Node *parse_factor(void);
Node *parse_print_statement(void);

int eval(Node *node);

#endif
