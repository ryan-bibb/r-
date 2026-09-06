#ifndef TOKEN_H
#define TOKEN_H

typedef enum
{
    TOKEN_NUMBER,    // 0
    TOKEN_PLUS,      // 1
    TOKEN_MINUS,     // 2
    TOKEN_MULTIPLY,  // 3
    TOKEN_DIVIDE,    // 4
    TOKEN_LPARAN,    // 5
    TOKEN_RPARAN,    // 6
    TOKEN_KW_PRINT,  // 7
    TOKEN_EOF,       // 8
} TokenType;

typedef struct
{
    TokenType type;
    int value; // tokens of type plus and eof still use val it  will waste a four bytes
} Token;

extern Token tokens[256];
extern int token_count;

void lexer(const char *src);
const char *token_type_name(TokenType type);

#endif
