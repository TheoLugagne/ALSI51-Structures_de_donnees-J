#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdbool.h>
#include "expressions/operator.h"

typedef enum {
    NUMBER, OPERATOR, PARENTHESIS, VARIABLE, STRING
} e_token_type;

typedef union {
    int val;
    operator_type op;
    bool paren_type;
    char var;
    char* string;
} u_token_content;

typedef struct {
    e_token_type type;
    u_token_content content;
} t_expr_token;

// Returns a token of type NUMBER containing the value val
t_expr_token token_of_int(int val);

// Returns a token of type OPERATOR containing the operator op
t_expr_token token_of_operator(operator_type op);

// Returns a token of type PARENTHESIS containing the parenthesis described by c = '(' or ')'
t_expr_token token_of_parenthesis(char c);

// Returns a token of type VARIABLE containing the variable var (a char between 'a' and 'z')
t_expr_token token_of_variable(char var);

// Returns a token of type STRING containing the value string
t_expr_token token_of_string(char* string);

// Returns true if the token is a left parenthesis
bool is_left_parenthesis(const t_expr_token *t);

// Returns true if the token is a right parenthesis
bool is_right_parenthesis(const t_expr_token *t);

// Returns true if the token has NUMBER or VARIABLE type
bool is_number_or_var(const t_expr_token *t);

// Prints the given token
void print_token(const t_expr_token *token);

// Prints the given token in the given file
void print_token_file(FILE *file, const t_expr_token *token);

#endif