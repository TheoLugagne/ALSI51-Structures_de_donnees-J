
#include <stdio.h>
#include <stdlib.h>

#include "program/program.h"
#include "program/lexical.h"

#include "program/parser.h"


void get_expr_rpn(t_expr_rpn *expr, const t_prog_token_list *list, unsigned int *i) {
    t_prog_token token = ptl_get(list, *i);
    if (token.token_type != PT_EXPR) {
        printf("Expression expected\n");
        *i = (unsigned int) (-1);
        return;
    }
    *expr = token.content.expr_rpn;
    (*i)++;
}

bool is_token_expr_or_string(const t_prog_token *token) {
    return token->token_type == PT_EXPR || token->token_type == PT_STRING;
}

static bool is_else = false;

t_ast *parse_aux(const t_prog_token_list *list, unsigned int *i) {

    if (*i >= list->size)
        return NULL;

    t_ast *prog = malloc(sizeof(t_ast)); // Current node of the AST
    u_statement statement;
    const t_prog_token token = ptl_get(list, *i);
    switch (token.token_type) {
        case PT_VAR: {
            prog->command = Assignment;
            t_assignment_statement st;
            st.var = token.content.var;
            *i = *i+2;
            get_expr_rpn(&st.expr, list, i);
            statement.assignment_st = st;
            prog->statement = statement;
            // prog->... = ...
            break;
        }
        case PT_KEYWORD: {
            switch (token.content.keyword) {
                case KW_PRINT: {
                    prog->command = Print;
                    t_print_statement st;
                    (*i)++;
                    const t_prog_token print_expr_token = ptl_get(list, *i);
                    if (!is_token_expr_or_string(&print_expr_token)) {
                        printf("Expression expected\n");
                        *i = (unsigned int) (-1);
                        break;
                    }
                    if (print_expr_token.token_type == PT_EXPR) {
                        st.expr_type = RPN;
                        get_expr_rpn(&st.expr, list, i);
                    } else {
                        st.expr_type = STR;
                        st.string = print_expr_token.content.expr;
                        (*i)++;
                    }
                    statement.print_st = st;
                    prog->statement = statement;
                    break;
                }
                case KW_RETURN: {
                    prog->command = Return;
                    t_return_statement st;
                    (*i)++;
                    get_expr_rpn(&st.expr, list, i);
                    statement.return_st = st;
                    prog->statement = statement;
                    break;
                }
                case KW_IF: {
                    prog->command = If;
                    t_if_statement st;
                    (*i)++;
                    get_expr_rpn(&st.cond, list, i);
                    st.if_true  = parse_aux(list, i);
                    if (is_else) {
                        is_else = false;
                        st.if_false = parse_aux(list, i);
                    } else {
                        st.if_false = NULL;
                    }
                    statement.if_st = st;
                    prog->statement = statement;
                    break;
                }
                case KW_WHILE: {
                    prog->command = While;
                    t_while_statement st;
                    (*i)++;
                    get_expr_rpn(&st.cond, list, i);
                    st.block = parse_aux(list, i);
                    statement.while_st = st;
                    prog->statement = statement;
                    break;
                }
                case KW_ENDBLOCK: {
                    (*i)++;
                    prog->next = NULL;
                    return NULL;
                }
                case KW_ELSE: {
                    (*i)++;
                    is_else = true;
                    prog->next = NULL;
                    return NULL;
                }
                case KW_FOR:
                    prog->command = For;
                    t_for_statement st;
                    (*i)++;
                    t_prog_token init_token = ptl_get(list, *i);
                    if (init_token.token_type == PT_VAR) {
                        st.init_type = VAR;
                        st.init.var = init_token.content.var;
                        (*i)++;
                    } else {
                        st.init_type = ASSIGNMENT;
                        t_assignment_statement st_assign;
                        st_assign.var = init_token.content.var;
                        *i = *i+2;
                        get_expr_rpn(&st_assign.expr, list, i);
                        st.init.assignment = st_assign;
                    }
                    get_expr_rpn(&st.cond, list, i);
                    get_expr_rpn(&st.expr, list, i);
                    st.block = parse_aux(list, i);
                    statement.for_st = st;
                    prog->statement = statement;
                    break;
                default:
                    printf("Syntax error: wrong keyword ");
                    print_keyword(token.content.keyword);
                    printf("\n");
                    break;
            }
            break;
        }
        case PT_STRING:
            break;
        default: {
            printf("Syntax error: wrong token type\n");
            break;
        }
    }
    if (*i == (unsigned int) (-1)) {
        //free(prog);
        return NULL;
    }
    prog->next = parse_aux(list, i);
    return prog;
}

t_ast *parse(const t_prog_token_list *list) {

    if (list->size == 0) return NULL;

    unsigned int i = 0; // index in the list
    return parse_aux(list, &i);
}