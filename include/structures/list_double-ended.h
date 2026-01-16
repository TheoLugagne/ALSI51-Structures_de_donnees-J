#ifndef LIST_DE_LINKED_H
#define LIST_DE_LINKED_H

#include <stdio.h>
#include "expressions/expr_token.h"

typedef t_expr_token T;

typedef struct s_cell {
    struct s_cell *next;
    T value;
} t_cell;

typedef struct {
    t_cell *head;
    t_cell *tail;
    int size;
    int num;
} t_list;

/////

t_list create_empty_list();

int length(const t_list *list);

T get(const t_list *list, int index);

void set(t_list *list, int index, T val);

void push_front(t_list *list, T val);

void push_back(t_list *list, T val);

void insert(t_list *list, int index, T val);

void delete_at(t_list *list, int index);

void print_list(const t_list *list);

void print_list_file(FILE *file, const t_list *list);

void destroy_list(t_list *list);

#endif