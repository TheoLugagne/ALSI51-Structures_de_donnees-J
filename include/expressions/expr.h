#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include "expressions/expr_token.h"
#include "structures/queue.h"

typedef struct {
    t_list list;
} t_expr;

// Returns true if the expression has no token left
bool is_empty_expr(const t_expr *expr);

// Adds a token at the back of the expression
void add_token(t_expr *expr, T token);

// Returns the token at the front of the expression and deletes it from the expression
T get_next_token(t_expr *expr);

// Prints the content of the expression
void print_expr(const t_expr *expr);

// Prints the content of the expression in the given file
void print_expr_file(FILE *file, const t_expr *expr);

// Returns the int at the beginning of the string pointed at by p_s
// Moves p_s past the int
int parse_int(const char **p_s);

// Returns the string between " " at the beginning of the string pointed at by p_s
// Moves p_s past the string
char* parse_string(const char **p_s);

// Converts the string s to an expression of type t_expr
t_expr parse_expr(const char **s);

typedef struct {
    t_expr expr;
} t_expr_rpn;

// Returns the result of the evaluation of the expression expr, in Reverse Polish notation
int eval_rpn(const int var_table[], const t_expr_rpn *expr);

// Converts an expression in infix notation to Reverse Polish notation
t_expr_rpn shunting_yard(t_expr *expr);

// Destructor
void destroy_expr(t_expr *expr);
void destroy_expr_rpn(t_expr_rpn *expr_rpn);

#endif
