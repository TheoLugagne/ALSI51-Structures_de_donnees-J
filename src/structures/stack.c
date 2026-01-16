#include "structures/stack.h"

t_stack create_empty_stack() {
    t_stack stack;
    stack.list = create_empty_list();
    return stack;
}

// Returns true if the stack is empty
bool is_empty_stack(const t_stack *stack) {
    return length(&stack->list) == 0;
}

// Pushes the value val to the top of the stack
void push(t_stack *stack, T val) {
    push_front(&stack->list, val);
}

// Returns the value at the top of the stack
T get_top(const t_stack *stack) {
    return get(&stack->list, 0);
}

// Returns the value at the top of the stack and deletes it from the stack
T pop(t_stack *stack) {
    T t = get_top(stack);
    delete_at(&stack->list, 0);
    return t;
}

// Destroys the stack
void destroy_stack(t_stack *stack) {
    destroy_list(&stack->list);
}

// Prints the content of the stack
void print_stack(const t_stack *stack) {
    print_list(&stack->list);
}