#include <stdio.h>
#include <stdlib.h>

#include "expressions/operator.h"

// Returns the operator corresponding to the character c
operator_type operator_of_char(char c) {
    switch(c) {
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MULT;
        case '/':
            return DIV;
        case '^':
            return EXP;
        case '<':
            return LESS;
        case '>':
            return GREATER;
        case '=':
            return EQUAL;
        case '!':
            return DIFF;
        case 'L':
            return LEQ;
        case 'G':
            return GEQ;
        case '&':
            return AND;
        case '|':
            return OR;
        case 'X':
            return XOR;
        case 'N':
            return NOT;
        default:
            exit(EXIT_FAILURE);
    }
}

// Returns a ^ b
int fast_exp(int a, int b) {
    if (b < 3) {
        return b == 2 ? a * a : (b ? a : 1);
    }
    int acc = 1;
    while (b > 0) {
        acc *= (b % 2) ? a : 1;
        a *= a;
        b /= 2;
    }
    return acc;
}

// Returns a op b
int apply_op(operator_type op, int a, int b) {
    switch (op) {
        case ADD:
            return a + b;
        case SUB:
            return a - b;
        case MULT:
            return a * b;
        case DIV:
            if (a == 0) {
                fprintf(stderr, "Division by zero\n");
                exit(EXIT_FAILURE);
            }
            return b / a;
        case EXP:
            return fast_exp(a, b);
        case LESS:
            return a < b;
        case GREATER:
            return a > b;
        case LEQ:
            return a <= b;
        case GEQ:
            return a >= b;
        case EQUAL:
            return a == b;
        case DIFF:
            return a != b;
        case AND:
            return a && b;
        case OR:
            return a || b;
        case XOR:
            return (a || b) && !(a && b); // 1 0 | 0 1 -> 1 ; 1 1 | 0 0 -> 0
        case NOT:
            return !a; // b is ignored
        default:
            fprintf(stderr, "Unknown operator in apply_op\n");
            exit(EXIT_FAILURE);
    }
}

void print_operator(operator_type op) {
    char c = '=';
    switch (op) {
        case ADD:
            c = '+';
            break;
        case SUB:
            c = '-';
            break;
        case MULT:
            c = '*';
            break;
        case DIV:
            c = '/';
            break;
        case EXP:
            c = '^';
            break;
        case LESS:
            c = '<';
            break;
        case GREATER:
            c = '>';
            break;
        case AND:
            c = '&';
            break;
        case OR:
            c = '|';
            break;
        case XOR:
            c = 'X';
            break;
        case NOT:
            c = 'N';
            break;
        case LEQ:
            printf("<");
            break;
        case GEQ:
            printf(">");
            break;
        case EQUAL:
            printf("=");
            break;
        case DIFF:
            printf("!");
            break;
    }
    printf("%c", c);
}

void print_operator_file(FILE *file, operator_type op) {
    char c = '=';
    switch (op) {
        case ADD:
            c = '+';
            break;
        case SUB:
            c = '-';
            break;
        case MULT:
            c = '*';
            break;
        case DIV:
            c = '/';
            break;
        case EXP:
            c = '^';
            break;
        case LESS:
            c = '<';
            break;
        case GREATER:
            c = '>';
            break;
        case AND:
            c = '&';
            break;
        case OR:
            c = '|';
            break;
        case XOR:
            c = 'X';
            break;
        case NOT:
            c = 'N';
            break;
        case LEQ:
            fprintf(file, "<");
            break;
        case GEQ:
            fprintf(file, ">");
            break;
        case EQUAL:
            fprintf(file, "=");
            break;
        case DIFF:
            fprintf(file, "!");
            break;
    }
    fprintf(file, "%c", c);
}



// Returns true if op2 takes priority over op1 in (a op2 b op1 c)
bool takes_priority(operator_type op1, operator_type op2) {
    //return (op1 != EXP) && (op2 == DIV || op2 == EXP || op1 == ADD || op1 == SUB)
      return (op1 != EXP &&
            op2 != LESS && op2 != GREATER && op2 != LEQ && op2 != GEQ && op2 != EQUAL && op2 != DIFF)
        &&
        (op2 == EXP
        || op1 == LESS || op1 == GREATER || op1 == LEQ || op1 == GEQ || op1 == EQUAL || op1 == DIFF
        || op2 == DIV || op2 == EXP || op1 == ADD || op1 == SUB);
}
