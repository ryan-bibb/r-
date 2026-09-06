#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "ast.h"

int main()
{
    const char *path = "/Users/ryanbibb/Desktop/dev/compiler/source.txt";
    FILE *source_file = fopen(path, "r");

    if (!source_file)
    {
        printf("Unable to read source code\n");
        exit(1);
    }

    // move file position indicator to end, get size/current pos, then move pos back to start
    fseek(source_file, 0, SEEK_END);
    long size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, source_file);
    buffer[size] = '\0';

    lexer(buffer);

    for (int i = 0; i < token_count; i++)
    {
        printf("Token type: %s -- Token value: %d\n", token_type_name(tokens[i].type), tokens[i].value);
    }
    
    printf("========================================\n");

    Node *ast = parse_statement();
    expect(TOKEN_EOF);
    eval(ast);
}
