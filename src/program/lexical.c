#include "program/lexical.h"

#include <stdio.h>

void print_keyword(const e_keyword keyword) {
    switch (keyword) {
        case KW_ASSIGN:
            printf("ASSIGN");
            break;
        case KW_IF:
            printf("IF");
            break;
        case KW_ELSE:
            printf("ELSE");
            break;
        case KW_WHILE:
            printf("WHILE");
            break;
        case KW_PRINT:
            printf("PRINT");
            break;
        case KW_RETURN:
            printf("RETURN");
            break;
        case KW_ENDBLOCK:
            printf("ENDBLOCK");
            break;
    }
}

void print_prog_token(const t_prog_token *token) {
    switch (token->token_type) {
        case PT_VAR:
            printf("Var(%c)", token->content.var);
            break;
        case PT_EXPR:
            printf("Expr(");
            print_expr(&token->content.expr_rpn.expr);
            printf(")");
            break;
        case PT_STRING:
            printf("StrExpr(");
            print_expr(&token->content.expr);
            printf(")");
            break;
        case PT_KEYWORD:
            print_keyword(token->content.keyword);
            break;
    }
}

void destroy_prog_token(t_prog_token *token) {
    destroy_expr_rpn(&token->content.expr_rpn);
}