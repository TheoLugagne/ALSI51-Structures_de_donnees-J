#include "expressions/expr.h"
#include "structures/stack.h"
#include <stdio.h>
#include <stdlib.h>

// Returns true if the expression has no token left
bool is_empty_expr(const t_expr *expr) {
    return expr->list.size == 0;
}

// Adds a token at the back of the expression
void add_token(t_expr *expr, T token) {
    push_back(&expr->list, token);
}

// Returns the token at the front of the expression and deletes it from the expression 
T get_next_token(t_expr *expr) {
    T elt = expr->list.head->value;
    expr->list.head = expr->list.head->next;
    expr->list.size--;
    return elt;
}

// Prints the content of the expression
void print_expr(const t_expr *expr) {
    print_list(&expr->list);
}

void print_expr_file(FILE *file, const t_expr *expr) {
    print_list_file(file, &expr->list);
}

// Returns the int at the beginning of the string pointed at by p_s
// Moves p_s past the int
int parse_int(const char **p_s) {

    const char *s = *p_s;
    int n = (*s) - '0';
    s++;
    while (*s >= '0' && *s <= '9') {
        n = 10 * n + (*s - '0');
        s++;
    }
    *p_s = s - 1;
    return n;
}

// Converts the string s to an expression of type t_expr
t_expr parse_expr(const char **s) {

    t_expr expr;
    expr.list = create_empty_list();

    const char *p = *s;

    bool parsed_number = false;

    while (*p != '\0') {

        if (*p == ' ') {
            p++;
            continue;
        }

        t_expr_token token;
        if (*p >= 'a' && *p <= 'z') {
            token = token_of_variable(*p);
            parsed_number = true;
        }
        else if (*p >= '0' && *p <= '9') {
            int n = parse_int(&p);
            token = token_of_int(n);
            parsed_number = true;
        }
        // Unary minus
        else if (!parsed_number && *p == '-') {
            p++;
            if (*p >= '0' && *p <= '9') {
                int n = parse_int(&p);
                token = token_of_int(-n);
                parsed_number = true;
            }
            else {
                printf("parse_expr: wrong syntax (\"%s\")\n", p);
                exit(EXIT_FAILURE);
            }
        }
        // Operator
        else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '^') {
            token = token_of_operator(operator_of_char(*p));
            parsed_number = false;
        }
        else if (*p == '(' || *p == ')') {
            token = token_of_parenthesis(*p);
        }
        else if (*p == '<' || *p == '>') {
            if (*(p+1) == '=')
                token = token_of_operator(*p == '<' ? LEQ : GEQ);
            else
                token = token_of_operator(*p == '<' ? LESS : GREATER);
            p++;
        }
        else if ((*p == '!' || *p == '=') && *(p+1) == '=') {
            token = token_of_operator(*p == '=' ? EQUAL : DIFF);
            p++;
        }
        else {
            break;
        }

        add_token(&expr, token);
        p++;
    }

    *s = p;
    return expr;
}

void error_rpn() {
    printf("Error: expression is not in Reverse Polish notation\n");
    exit(EXIT_FAILURE);
}


////////////////////////////////////////////////////////////////////

// Returns the value of the variable var
int look_up_variable(const int var_table[], const char var) {
    return 0; // TODO
}

// Returns the value of the token t (of type NUMBER or VARIABLE)
int get_value(const int var_table[], const t_expr_token *t) {
    return 0; // TODO
}

// Returns the result of the evaluation of the expression expr, in Reverse Polish notation
int eval_rpn(const int var_table[], const t_expr_rpn *expr_rpn) {

    t_list list_copy = expr_rpn->expr.list;
    t_expr expr = { .list = list_copy };

    t_stack stack = create_empty_stack();

    while (!is_empty_expr(&expr)) {
        t_expr_token token = get_next_token(&expr);

        switch (token.type) {
            case NUMBER:
            case VARIABLE:
                // TODO
                break;
            
            case OPERATOR: {
                // TODO
                break;
            }

            case PARENTHESIS:
                // TODO
                break;
        }
    }

    // TODO

    destroy_stack(&stack);
    return 0; // TODO
}

// Converts an expression in infix notation to Reverse Polish notation
t_expr_rpn shunting_yard(t_expr *expr) {

    t_expr_rpn expr_rpn;
    expr_rpn.expr.list = create_empty_list();
    t_expr *output = &expr_rpn.expr;

    t_stack op_stack = create_empty_stack();

    while (!is_empty_expr(expr)) {
        t_expr_token t = get_next_token(expr);
        switch (t.type) {
            case NUMBER:
            case VARIABLE:
                add_token(output, t);
                break;
            case OPERATOR:
                while (!is_empty_stack(&op_stack)) {
                    t_expr_token t2 = get_top(&op_stack);
                    if (t2.type != OPERATOR || !takes_priority(t.content.op, t2.content.op)) {
                        break;
                    }
                    add_token(output, t2);
                    pop(&op_stack);
                }
                push(&op_stack, t);
                break;
            case PARENTHESIS:
                if (is_left_parenthesis(&t)) {
                    push(&op_stack, t);
                }
                else {
                    bool parenthesis_found = false;
                    while (!is_empty_stack(&op_stack)) {
                        t_expr_token t2 = pop(&op_stack);
                        if (t2.type == PARENTHESIS && is_left_parenthesis(&t2)) {
                            parenthesis_found = true;
                            break;
                        }
                        add_token(output, t2);
                    }
                    if (!parenthesis_found) {
                        printf("Error, missing parenthesis\n");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
        }
    }

    while (!is_empty_stack(&op_stack)) {
        t_expr_token t = pop(&op_stack);
        if (t.type != OPERATOR) {
            printf("Error, expression is wrongly formed\n");
            exit(EXIT_FAILURE);
        }
        add_token(output, t);
    }

    destroy_expr(expr);
    destroy_stack(&op_stack);
    return expr_rpn;
}

void destroy_expr(t_expr *expr) {
    // TODO
}

void destroy_expr_rpn(t_expr_rpn *expr_rpn) {
    // TODO
}