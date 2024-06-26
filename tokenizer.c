#ifndef TOKENIZER_C
#define TOKENIZER_C

#include "containers/array.h"

typedef enum TokenType {
    NONE = 0,
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PAREN_L,
    PAREN_R
} TokenType;

typedef struct Token {
    TokenType type;
    int index;
    int value;
} Token;

INSTANTIATE_DARRAY(Tokens, Token);


const char *TokenTypeToString(TokenType type);

/*!
* @return 0 on success, non-zero otherwise
*/
int TokenizeExpression(Tokens *tokens, const char *expr);
void printTokens(Tokens *tokens);

#ifdef TOKENIZER_IMPLEMENTATION

#include "containers/string.h"

const char *TokenTypeToString(TokenType type) {
    switch (type) {
        case NONE:    return "NONE";
        case NUMBER:  return "NUMBER";
        case PLUS:    return "PLUS";
        case MINUS:   return "MINUS";
        case STAR:    return "STAR";
        case SLASH:   return "SLASH";
        case PAREN_L: return "PAREN_L";
        case PAREN_R: return "PAREN_R";
        default:      return "<unknown>";
    }
}

int TokenizeExpression(Tokens *tokens, const char *expr) {
    Token token = {0};
    DString number_buffer = {0};
    const char *original_expr = expr; // for error message
    int result = 0;

    for (int i = 0; *expr;) {
        token.type = NONE;
        token.index = i;

        switch (*expr) {
            case '+':
                token.type = PLUS;
                break;
            case '-':
                token.type = MINUS;
                break;
            case '*':
                token.type = STAR;
                break;
            case '/':
                token.type = SLASH;
                break;
            case '(':
                token.type = PAREN_L;
                break;
            case ')':
                token.type = PAREN_R;
                break;
        }

        if (token.type != NONE) {
            DArrayAppend(tokens, token);
            i++, expr++;
            continue;
        }
        
        if (isdigit(*expr)) {
            token.type = NUMBER;

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
            result = 1;
            goto defer;
        }
    }

defer:
    DStringFree(&number_buffer);
    return result;
}

void printTokens(Tokens *tokens) {
    for (int i = 0; i < tokens->count; ++i) {
        Token token = tokens->items[i];
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
            case PAREN_L:
                printf("[TOKEN] [idx %d] Opening Parenthesis\n", token.index);
                break;
            case PAREN_R:
                printf("[TOKEN] [idx %d] Closing Parenthesis\n", token.index);
                break;
            default:
                printf("[TOKEN] INVALID (type:%d)\n", token.type);
        }
    }
}

#endif // TOKENIZER_IMPLEMENTATION
#endif // TOKENIZER_C