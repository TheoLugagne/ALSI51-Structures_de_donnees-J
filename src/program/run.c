#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "program/program.h"

// Recursive function, evaluates the program
// Returns true if a Return statement was reached, stop the execution
// Returns false if the end of a block was reached
bool run_aux(int var_value[], const t_ast *prog) {

    if (prog == NULL)
        return false;

    switch (prog->command) {
        case Return: {
            const t_return_statement st = prog->statement.return_st;
            var_value[26] = eval_rpn(var_value, &st.expr);
            return true;
        }
        case Assignment: {
            const t_assignment_statement st = prog->statement.assignment_st;
            var_value[(unsigned char)st.var - 'a'] = eval_rpn(var_value, &st.expr);
            break;
        }
        case Print: {
            const t_print_statement st = prog->statement.print_st;
            if (st.expr_type == RPN) {
                fprintf(stdout, "%d\n", eval_rpn(var_value, &st.expr));
            }
            if (st.expr_type == STR) {
                fprintf(stdout, "%s\n", eval_string_expr(&st.string));
            }
            break;
        }
        case If: {
            const t_if_statement st = prog->statement.if_st;
            bool if_res = false;
            if (eval_rpn(var_value, &st.cond)) {
                if_res = run_aux(var_value, st.if_true);
            } else {
                if_res = run_aux(var_value, st.if_false);
            }
            if (if_res) return true;
            break;
        }
        case While: {
            const t_while_statement st = prog->statement.while_st;
            bool while_res = false;
            while (eval_rpn(var_value, &st.cond)) {
                while_res = run_aux(var_value, st.block);
                if (while_res) return true;
            }
            break;
        }
        case For: {
            const t_for_statement st = prog->statement.for_st;
            const char var = st.init_type == VAR ? st.init.var : st.init.assignment.var;
            if (st.init_type == ASSIGNMENT) {
                var_value[(unsigned char)var - 'a'] = eval_rpn(var_value, &st.init.assignment.expr);
            }
            while (eval_rpn(var_value, &st.cond)) {
                bool for_res = false;
                for_res = run_aux(var_value, st.block);
                if (for_res) return true;
                var_value[(unsigned char)var - 'a'] = eval_rpn(var_value, &st.expr);
            }
            break;
        }
        default: {
            fprintf(stderr , "Syntax error, Unrecognize statement\n");
            exit(EXIT_FAILURE);
        }
    }
    return run_aux(var_value, prog->next);
}

void run(const t_ast *prog) {
    int var_value[27];
    for (int i = 0; i < 27; i++) {
        var_value[i] = 0;
    }
    run_aux(var_value, prog);
    fprintf(stdout, "return : %d\n", var_value[26]);
    // for (int i = 0; i < 27; i++) {
    //     fprintf(stdout, "%d\n", var_value[i]);
    // }
}