#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "containers/array.h"
#include "containers/string.h"
#include "containers/tree.h"

#define TOKENIZER_IMPLEMENTATION
#include "tokenizer.c"


TreeNode *parseExpression(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count);
TreeNode *      parseTerm(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count);
TreeNode *    parseFactor(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count);

#define SETPTR(ptr, val) if (ptr) *ptr = val

TreeNode *parseExpression(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count) {
    TreeNode *result_tree = NULL;
    size_t idx = 0;
    TreeNode *term = NULL, *operation = NULL, *expression = NULL;

    if (count == 0) {
        printf("[PARSER ERROR] Empty expression\n");
        goto failure;
    }

    size_t parsed_count;
    Token *peek_token;

    term = parseTerm(tokens, count, raw_expression, &parsed_count);
    idx += parsed_count;

    if (!term) {
        goto failure;
    }

    if (idx == count) {
        result_tree = term;
        goto success;
    }

    peek_token = tokens + idx;
    if (peek_token->type != PLUS && peek_token->type != MINUS) {
        printf("[PARSER ERROR] Expected PLUS or MINUS but got %s\n", TokenTypeToString(peek_token->type));
        puts(raw_expression);
        printf("%*c^\n", peek_token->index ? peek_token->index + 1 : peek_token->index, '\0');
        
        goto failure;
    }

    Token *operation_token = malloc(sizeof(Token));
    memcpy(operation_token, peek_token, sizeof(Token));
    operation = TreeAddChildValue(NULL, operation_token, sizeof(Token));

    idx += 1;

    expression = parseExpression(tokens + idx, count - idx, raw_expression, &parsed_count);
    idx += parsed_count;

    if (!expression) {
        goto failure;
    }

    TreeAddChildNode(operation, term);
    TreeAddChildNode(operation, expression);
    result_tree = operation;
    goto success;

failure:
    TreeFree(term);
    TreeFree(operation);
    TreeFree(expression);
success:
    SETPTR(tokens_parsed_count, idx);
    return result_tree;
}

TreeNode *parseTerm(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count) {
    TreeNode *result_tree = NULL;
    size_t idx = 0;
    TreeNode *factor = NULL, *operation = NULL, *term = NULL;

    if (count == 0) {
        printf("[PARSER ERROR] Empty term\n");
        goto failure;
    }

    size_t parsed_count;
    Token *peek_token;

    factor = parseFactor(tokens, count, raw_expression, &parsed_count);
    idx += parsed_count;

    if (!factor) {
        goto failure;
    }

    if (idx == count) {
        result_tree = factor;
        goto success;
    }

    peek_token = tokens + idx;
    if (peek_token->type == PLUS || peek_token->type == MINUS) {
        result_tree = factor;
        goto success;
    }
    else if (peek_token->type != STAR && peek_token->type != SLASH) {
        printf("[PARSER ERROR] Expected operation but got %s\n", TokenTypeToString(peek_token->type));
        puts(raw_expression);
        printf("%*c^\n", peek_token->index ? peek_token->index + 1 : peek_token->index, '\0');
        
        goto failure;
    }

    Token *operation_token = malloc(sizeof(Token));
    memcpy(operation_token, peek_token, sizeof(Token));
    operation = TreeAddChildValue(NULL, operation_token, sizeof(Token));

    idx += 1;

    term = parseTerm(tokens + idx, count - idx, raw_expression, &parsed_count);
    idx += parsed_count;

    if (!term) {
        goto failure;
    }

    TreeAddChildNode(operation, factor);
    TreeAddChildNode(operation, term);
    result_tree = operation;
    goto success;

failure:
    TreeFree(factor);
    TreeFree(operation);
    TreeFree(term);
success:
    SETPTR(tokens_parsed_count, idx);
    return result_tree;
}

TreeNode *parseFactor(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count) {
    TreeNode *result_tree = NULL;
    size_t idx = 0;

    if (count == 0) {
        printf("[PARSER ERROR] Empty factor\n");
        SETPTR(tokens_parsed_count, idx);
        return NULL;
    }

    if (tokens->type != NUMBER) {
        printf("[PARSER ERROR] Expected NUMBER but got %s\n", TokenTypeToString(tokens->type));
        puts(raw_expression);
        printf("%*c^\n", tokens->index ? tokens->index + 1 : tokens->index, '\0');
        return NULL;
    }

    Token *number_token = malloc(sizeof(Token));
    memcpy(number_token, tokens, sizeof(Token));
    result_tree = TreeAddChildValue(NULL, number_token, sizeof(Token));

    idx += 1;
    SETPTR(tokens_parsed_count, idx);

    return result_tree;
}

void printTree(TreeNode *node, int indent) {
    if (!node) return;

    printf("%*c%s", indent, '\0', TokenTypeToString(DEREF_AS(Token, node->value).type));
    if (DEREF_AS(Token, node->value).type == NUMBER) {
        printf(" %d", DEREF_AS(Token, node->value).value);
    }
    putc(10, stdout);

    for (size_t i = 0; i < node->children.count; ++i) {
        printTree(node->children.items[i], indent + 4);
    };
}

int main() {
    const char *expression = "1 + 2 + 3 * 45 / 51 - 452 + 42 * 32";

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

    DArrayFree(&tokens);
    TreeFree(parsedExpression);

    printf("No segfault yay!\n");

    return 0;
}

                           