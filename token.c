#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

const char *KEY_WORDS[1] = {"print"};

Token tokens[256];
int token_count = 0;

// **********************************************************************************
// right now there can be back to back operators, this should not be allowed
void lexer(const char *source)
{
    int i = 0;

    while (source[i] != '\0')
    {
        if (isspace(source[i])) // white space
        {
            i++;
        }
        else // non white space
        {
            // number
            if (isdigit(source[i]))
            {
                int value = 0;

                while (isdigit(source[i]))
                {
                    value = value * 10 + (source[i] - '0');
                    i++;
                }

                tokens[token_count++] = (Token){TOKEN_NUMBER, value};
            }
            else if (ispunct(source[i])) // punctionation: +, -, /, *
            {
                if (source[i] == '+')
                {
                    tokens[token_count++] = (Token){TOKEN_PLUS, 0};
                }
                else if (source[i] == '-')
                {
                    tokens[token_count++] = (Token){TOKEN_MINUS, 0};
                }
                else if (source[i] == '*')
                {
                    tokens[token_count++] = (Token){TOKEN_MULTIPLY, 0};
                }
                else if (source[i] == '/')
                {
                    tokens[token_count++] = (Token){TOKEN_DIVIDE, 0};
                }
                else if (source[i] == '(')
                {
                    tokens[token_count++] = (Token){TOKEN_LPARAN, 0};
                }
                else if (source[i] == ')')
                {
                    tokens[token_count++] = (Token){TOKEN_RPARAN, 0};
                }
                else
                {
                    printf("Unknown operator: %c\n", source[i]);
                    exit(1);
                }
                // TODO: add other puncation later

                i++;
            }
            else if (isalpha(source[i]))
            {
                char key_word[32];
                int key_word_size = 0;
                int is_valid = 0;

                while (source[i] != '\0' && isalpha(source[i]))
                {
                    key_word[key_word_size++] = source[i++];
                }

                key_word[key_word_size] = '\0';

                for (int j = 0; j < sizeof(KEY_WORDS) / 8; j++) // WARNING: this is risky to use sizeof on keywords -> assuming 8 bytes and a 64 bit system
                {
                    if (strcmp(KEY_WORDS[j], key_word) == 0)
                    {
                        is_valid = 1;
                    }
                }

                // valid keywords
                if (is_valid)
                {
                    if (strcmp(key_word, "print") == 0)
                    {
                        tokens[token_count++] = (Token){TOKEN_KW_PRINT, 0};
                    }
                }
                else
                {
                    printf("Unknown keyword: %s\n", key_word);
                    exit(1);
                }
            }
            else
            {
                fprintf(stderr, "Unexpected character: %c\n", source[i]);
                exit(1);
            }
        }
    }

    tokens[token_count++] = (Token){TOKEN_EOF, 0};
}

// **********************************************************************************

const char *token_type_name(TokenType type)
{
    switch (type)
    {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_LPARAN: return "LEFT PARANTHESES";
        case TOKEN_RPARAN: return "RIGHT PARANTHESES";
        case TOKEN_KW_PRINT: return "PRINT KEYWORD";
        case TOKEN_EOF: return "EOF";
        default: return "UNKOWN";
    }
}
