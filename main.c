#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "containers/array.h"
#include "containers/string.h"

typedef enum {
    NONE = 0,
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    BRACE_L,
    BRACE_R
} TokenType;

typedef struct {
    TokenType type;
    int index;
    int value;
} Token;

INSTANTIATE_DARRAY(Tokens, Token);

void TokenizeExpression(Tokens *tokens, const char *expr) {
    Token token = {0};
    DString number_buffer = {0};
    const char *original_expr = expr; // for error message

    for (int i = 0; *expr;) {
        token.type = NONE;

        switch (*expr) {
            case '+':
                token.type = PLUS;
                token.index = i;
                break;
            case '-':
                token.type = MINUS;
                token.index = i;
                break;
            case '*':
                token.type = STAR;
                token.index = i;
                break;
            case '/':
                token.type = SLASH;
                token.index = i;
                break;
            case '(':
                token.type = BRACE_L;
                token.index = i;
                break;
            case ')':
                token.type = BRACE_R;
                token.index = i;
                break;
            case ' ':

        }

        if (token.type != NONE) {
            DArrayAppend(tokens, token);
            i++, expr++;
            continue;
        }
        
        if (isdigit(*expr)) {
            token.type = NUMBER;
            token.index = i;

            do {
                DStringAppend(&number_buffer, *expr);
                i++;
                expr++;
            } while (isdigit(*expr));

            token.value = atoi(number_buffer.items);

            number_buffer.count = 0;
            DArrayAppend(tokens, token);

        } else if (isspace(*expr)) {
            i++, expr++;
            continue;

        } else {
            printf("[TOKENIZER ERROR] Unexpected char '%c'(code: %d) at index %d\n", *expr, *expr, i);
            puts(original_expr);
            printf("%*c^\n\n", i ? i + 1 : i, '\0');
            goto defer;
        }
    }

defer:
    DStringFree(&number_buffer);
}

int main() {
    const char *expression = "1 + 2 + 3 * 45 / (51 + (452 + 42) * 32)";

    printf("Got expresssion: '%s'\n", expression);

    Tokens tokens = {0};
    TokenizeExpression(&tokens, expression);

    putc(10, stdout); // newline hi-hi-ha-ha

    for (int i = 0; i < tokens.count; ++i) {
        Token token = tokens.items[i];
        switch (token.type) {
            case NONE:
                printf("[TOKEN] [idx %d] None\n", token.index);
                break;
            case NUMBER:
                printf("[TOKEN] [idx %d] Number %d\n", token.index, token.value);
                break;
            case PLUS:
                printf("[TOKEN] [idx %d] Plus\n", token.index);
                break;
            case MINUS:
                printf("[TOKEN] [idx %d] Minus\n", token.index);
                break;
            case STAR:
                printf("[TOKEN] [idx %d] Star\n", token.index);
                break;
            case SLASH:
                printf("[TOKEN] [idx %d] Slash\n", token.index);
                break;
            case BRACE_L:
                printf("[TOKEN] [idx %d] Open Brace\n", token.index);
                break;
            case BRACE_R:
                printf("[TOKEN] [idx %d] Close Brace\n", token.index);
                break;
            default:
                printf("[TOKEN] INVALID");
        }
    }

    DArrayFree(&tokens);

    return 0;
}

                           