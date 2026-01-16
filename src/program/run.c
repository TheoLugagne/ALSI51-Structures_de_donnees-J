#include <stdio.h>
#include <stdbool.h>

#include "program/program.h"

// Recursive function, evaluates the program
// Returns true if a Return statement was reached, stop the execution
// Returns false if the end of a block was reached
bool run_aux(int var_value[], const t_ast *prog) {

    if (prog == NULL)
        return false;

    switch (prog->command) {

        case Return: {
            // prog->command == Return, so we assume that the statement has type t_return_statement
            t_return_statement st = prog->statement.return_st;
            // TODO
            return false; // TODO
        }
            // TODO
        default:
            break;
    }

    return false; // TODO
}

void run(t_ast *prog) {

    // TODO
}