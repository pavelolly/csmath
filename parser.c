#ifndef PARSER_C
#define PARSER_C

#include <stdio.h>

#include "containers/tree.h"
#include "tokenizer.c"

TreeNode *parseExpression(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count);
TreeNode *      parseTerm(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count);
TreeNode *    parseFactor(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count);
void printTree(TreeNode *node, int indent);


#ifdef PARSER_IMPLEMENTATION

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
    if (peek_token->type == BRACE_R) {
        result_tree = term;
        goto success;
    }
    if (peek_token->type != PLUS && peek_token->type != MINUS) {
        printf("[PARSER ERROR] Expected PLUS or MINUS or BRACE_R but got %s\n", TokenTypeToString(peek_token->type));
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
    result_tree = NULL;
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
    if (peek_token->type == PLUS || peek_token->type == MINUS || peek_token->type == BRACE_R) {
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
    result_tree = NULL;
success:
    SETPTR(tokens_parsed_count, idx);
    return result_tree;
}

TreeNode *parseFactor(Token *tokens, size_t count, const char *raw_expression, size_t *tokens_parsed_count) {
    TreeNode *result_tree = NULL;
    size_t idx = 0;

    if (count == 0) {
        printf("[PARSER ERROR] Empty factor\n");
        goto failure;
    }

    if (tokens->type == NUMBER) {
        Token *number_token = malloc(sizeof(Token));
        memcpy(number_token, tokens, sizeof(Token));
        result_tree = TreeAddChildValue(NULL, number_token, sizeof(Token));
        idx += 1;
    }
    else if (tokens->type == BRACE_L) {
        idx += 1;

        size_t parsed_count;
        result_tree = parseExpression(tokens + idx, count - idx, raw_expression, &parsed_count);
        
        idx += parsed_count;

        if (!result_tree) {
            goto failure;
        }

        if (idx == count) {
            printf("[PARSER ERROR] this opening brace doesn't have match\n");
            puts(raw_expression);
            printf("%*c^\n", tokens->index ? tokens->index + 1 : tokens->index, '\0');
            goto failure;
        }

        if (tokens[idx].type != BRACE_R) {
            printf("[PARSER ERROR] expected BRACE_R but got %s\n", TokenTypeToString(tokens[idx].type));
            puts(raw_expression);
            printf("%*c^\n", tokens[idx].index ? tokens[idx].index + 1 : tokens[idx].index, '\0');
            goto failure;
        }

        idx += 1;
    }
    else {
        printf("[PARSER ERROR] expected NUMBER ot BRACE_L but got %s\n", TokenTypeToString(tokens->type));
        puts(raw_expression);
        printf("%*c^\n", tokens->index ? tokens->index + 1 : tokens->index, '\0');
        goto failure;
    }

    goto success;
    
failure:
    TreeFree(result_tree);
    result_tree = NULL;
success:
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

#endif // PARSER_IMPEMENTATION
#endif // PARSER_C