#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#include "list_double-ended.h"

typedef struct {
    t_list list;
} t_stack;

t_stack create_empty_stack();

// Returns true if the stack is empty
bool is_empty_stack(const t_stack *stack);

// Pushes the value val to the top of the stack
void push(t_stack *stack, T val);

// Returns the value at the top of the stack
T get_top(const t_stack *stack);

// Returns the value at the top of the stack and deletes it from the stack
T pop(t_stack *stack);

// Destroys the stack
void destroy_stack(t_stack *stack);

// Prints the content of the stack
void print_stack(const t_stack *stack);

#endif