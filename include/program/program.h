#ifndef PROGRAM_H
#define PROGRAM_H

#include "expressions/expr.h"

typedef struct s_ast t_ast;

// Types of statements
typedef enum {
    Assignment, If, While, Return, Print, For
} e_statement_type;

typedef enum {
    RPN, STR
} e_print_expr_type;

typedef enum {
    VAR, ASSIGNMENT
} e_for_init_type;

// return [expr]
typedef struct {
    t_expr_rpn expr;
} t_return_statement;

// print [expr || string] -- only one is filled
typedef struct {
    e_print_expr_type expr_type;
    t_expr_rpn expr;
    t_expr string;
} t_print_statement;

// [var] = [expr]
typedef struct {
    char var;
    t_expr_rpn expr;
} t_assignment_statement;

// if [cond]
//     [if_true]
// else
//     [if_false]
typedef struct {
    t_expr_rpn cond;
    t_ast *if_true;
    t_ast *if_false;
} t_if_statement;

// while [cond]
//     [block]
typedef struct {
    t_expr_rpn cond;
    t_ast *block;
} t_while_statement;

// for ([init]; [cond]; [expr])
//    [block]
typedef union {
    char var;
    t_assignment_statement assignment;
} u_for_init;

typedef struct {
    e_for_init_type init_type;
    u_for_init init;
    t_expr_rpn cond;
    t_expr_rpn expr;
    t_ast *block;
} t_for_statement;

// union containing a statement
typedef union {
    t_return_statement        return_st;
    t_print_statement         print_st;
    t_assignment_statement    assignment_st;
    t_if_statement            if_st;
    t_while_statement         while_st;
    t_for_statement           for_st;
} u_statement;

// Recursive type for AST: a statement, its type, and a pointer to the next node of the AST
typedef struct s_ast {
    e_statement_type command;
    u_statement statement;
    struct s_ast *next;
} t_ast;

void print_ast(const t_ast *prog, const char *file_name);

// Parses and executes the program in the string s
void run_program(const char *s);

// Exports the ast of the code in a file prog.mmd
void export_program_ast(const char *s, const char *source_file_name);

// Destructor
void destroy_ast(t_ast *prog);

#endif