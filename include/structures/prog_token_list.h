#ifndef PROG_TOKEN_LIST_H
#define PROG_TOKEN_LIST_H

#include "program/lexical.h"

typedef t_prog_token T_ptl;
typedef struct {
    T_ptl *data;
    int size;
    int capacity;
} t_prog_token_list;

/////

t_prog_token_list ptl_create_empty_list();

T_ptl ptl_get(const t_prog_token_list *list, int index);

void ptl_set(t_prog_token_list *list, int index, T_ptl val);

void ptl_push_front(t_prog_token_list *list, T_ptl val);

void ptl_push_back(t_prog_token_list *list, T_ptl val);

void ptl_insert(t_prog_token_list *list, int index, T_ptl val);

void ptl_delete_at(t_prog_token_list *list, int index);

void ptl_print_list(const t_prog_token_list *list);

void ptl_destroy_list(t_prog_token_list *list);

#endif