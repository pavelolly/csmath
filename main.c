#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "containers/array.h"
#include "containers/string.h"
#include "containers/tree.h"

#define TOKENIZER_IMPLEMENTATION
#include "tokenizer.c"

#define PARSER_IMPLEMENTATION
#include "parser.c"


int main(void) {
    const char *expression_file_path = "expression.txt";
    DString expression_builder = {0};
    if (!DStringReadFile(&expression_builder, expression_file_path)) {
        printf("Could not read expression from '%s': %s\n", expression_file_path, strerror(errno), expression_builder.items);
        return 1;
    };
    const char *expression = expression_builder.items;

    printf("Got expresssion: '%s'\n\n", expression);

    Tokens tokens = {0};
    if (TokenizeExpression(&tokens, expression)) {
        printf("Failed to tokenize expression\n");
        return 1;
    }

    putc(10, stdout); // newline hi-hi-ha-ha
    printTokens(&tokens);

    putc(10, stdout);
    TreeNode *parsedExpression = parseExpression(tokens.items, tokens.count, expression, NULL);

    printTree(parsedExpression, 0);

// free section
    DStringFree(&expression_builder);
    DArrayFree(&tokens);
    TreeFree(parsedExpression);

    printf("No segfault yay!\n");

    return 0;
}

                           