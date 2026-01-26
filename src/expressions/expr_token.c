#include "expressions/expr_token.h"
#include <stdio.h>

t_expr_token token_of_int(int val) {
    t_expr_token t;
    t.type = NUMBER;
    t.content.val = val;
    return t;
}

t_expr_token token_of_operator(operator_type op) {
    t_expr_token t;
    t.type = OPERATOR;
    t.content.op = op;
    return t;
}

t_expr_token token_of_parenthesis(char c) {
    t_expr_token t;
    t.type = PARENTHESIS;
    t.content.paren_type = (c == '(');
    return t;
}

t_expr_token token_of_variable(char var) {
    t_expr_token t;
    t.type = VARIABLE;
    t.content.var = var;
    return t;
}

t_expr_token token_of_string(char* string) {
    t_expr_token t;
    t.type = STRING;
    t.content.string = string;
    return t;
}

// Returns true if the token is a left parenthesis
bool is_left_parenthesis(const t_expr_token *t) {
    return t->type == PARENTHESIS && t->content.paren_type;
}
// Returns true if the token is a right parenthesis
bool is_right_parenthesis(const t_expr_token *t) {
    return t->type == PARENTHESIS && !t->content.paren_type;
}

bool is_number_or_var(const t_expr_token *t) {
    return t->type == NUMBER || t->type == VARIABLE;
}

void print_token(const t_expr_token *token) {
    switch (token->type) {
        case NUMBER:
            printf("%d", token->content.val);
            break;
        case OPERATOR:
            print_operator(token->content.op);
            break;
        case PARENTHESIS:
            printf("%c", is_left_parenthesis(token) ? '(' : ')');
            break;
        case VARIABLE:
            printf("%c", token->content.var);
            break;
        case STRING:
            printf("%s", token->content.string);
            break;
    }
}

void print_token_file(FILE *file, const t_expr_token *token) {
    switch (token->type) {
        case NUMBER:
            fprintf(file, "%d", token->content.val);
            break;
        case OPERATOR:
            print_operator_file(file, token->content.op);
            break;
        case PARENTHESIS:
            fprintf(file, "%c", is_left_parenthesis(token) ? '(' : ')');
            break;
        case VARIABLE:
            fprintf(file, "%c", token->content.var);
            break;
        case STRING:
            fprintf(file, "%s", token->content.string);
            break;
    }
}