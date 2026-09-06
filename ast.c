#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int parser_position = 0;

// **********************************************************************************
// parser: expr := term (('+' | '-') term)*
//         term := factor (('*' | '/') factor)*
//         factor := NUMBER | '(' expr ')'

Token *peek(void)
{
    return &tokens[parser_position];
}

Token *expect(TokenType type)
{
    if (peek()->type != type)
    {
        fprintf(stderr, "Expected token %s but got %s\n", token_type_name(type), token_type_name(peek()->type));
        exit(1);
    }

    Token *token = peek();
    parser_position++;
    return token;
}

Node *new_number_node(int value)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_NUMBER;
    node->value = value;
    return node;
}

Node *new_binop_node(char operation, Node *left, Node *right)
{
    Node *node = malloc(sizeof(Node)); // use malloc to put this chunk of memory on the heap instead of the stack
    node->type = NODE_BINOP;
    node->operation = operation;
    node->left = left;
    node->right = right;
    return node;
}

Node *new_print_node(Node *expr)
{
    Node *node = malloc(sizeof(Node));
    node->type = NODE_PRINT;
    node->left = expr;
    return node;
}

Node *parse_print_statement(void)
{
    expect(TOKEN_KW_PRINT);
    expect(TOKEN_LPARAN);
    Node *expr = parse_expr();
    expect(TOKEN_RPARAN);

    return new_print_node(expr);
}

Node *parse_factor(void)
{
    if (peek()->type == TOKEN_LPARAN)
    {
        parser_position++;
        Node *node = parse_expr();
        expect(TOKEN_RPARAN);
        return node;
    }

    return new_number_node(expect(TOKEN_NUMBER)->value);
}

Node *parse_term(void)
{
    Node *left = parse_factor();

    while (peek()->type == TOKEN_MULTIPLY || peek()->type == TOKEN_DIVIDE)
    {
        Token *operation = peek(); // only safe because returning pointer to global tokens, not a local pointer
        char operation_char = (operation->type == TOKEN_MULTIPLY) ? '*' : '/';

        expect(operation->type);
        Node *right = parse_factor();
        left = new_binop_node(operation_char, left, right);
    }

    return left;
}

Node *parse_expr(void)
{
    Node *left = parse_term();

    while (peek()->type == TOKEN_PLUS || peek()->type == TOKEN_MINUS)
    {
        Token *operation = peek(); // only safe because returning pointer to global tokens, not a local pointer
        char operation_char = (operation->type == TOKEN_PLUS) ? '+' : '-';

        expect(operation->type);
        Node *right = parse_term();
        left = new_binop_node(operation_char, left, right);
    }

    return left;
}

Node *parse_statement(void)
{
    if (peek()->type == TOKEN_KW_PRINT)
    {
        return parse_print_statement();
    }
    else
    {
        return parse_expr();
    }
}

// **********************************************************************************
// evaluator

int eval(Node *node)
{
    // numbers
    if (node->type == NODE_NUMBER)
    {
        return node->value;
    }

    // operators
    if (node->type == NODE_BINOP)
    {
        if (node->operation == '+')
        {
            return eval(node->left) + eval(node->right);
        }
        else if (node->operation == '-')
        {
            return eval(node->left) - eval(node->right);
        }
        else if (node->operation == '*')
        {
            return eval(node->left) * eval(node->right);
        }
        else // if (node->operation == '/')
        {
            return eval(node->left) / eval(node->right);
        }
    }

    // keywords
    if (node->type == NODE_PRINT)
    {
        // this will have to change from %d to support strings 
        printf("%d\n", eval(node->left));
        return 0;
    }

    // unknown node type
    exit(1);
}
