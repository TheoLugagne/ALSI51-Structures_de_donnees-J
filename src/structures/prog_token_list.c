#include <stdio.h>
#include <stdlib.h>

#include "structures/prog_token_list.h"

#define INIT_CAPACITY 10


t_prog_token_list ptl_create_empty_list() {
    t_prog_token_list l;
    l.data = (T_ptl*) malloc(INIT_CAPACITY * sizeof(T_ptl));
    l.size = 0;
    l.capacity = INIT_CAPACITY;
    return l;
}

void ptl_realloc_list(t_prog_token_list *list) {
    const unsigned int new_capacity = 2 * list->capacity;
    T_ptl *new_data = (T_ptl*) malloc(new_capacity * sizeof(T_ptl));
    for (unsigned int i = 0; i < list->size; i++) {
        new_data[i] = list->data[i];
    }
    free(list->data);
    list->data = new_data;
    list->capacity = new_capacity;
}

// Shifts the elements of indices index_start .. size-1 one position to the right
// It is assumed that capacity >= size + 1
void ptl_shift_right(t_prog_token_list *list, int index_start) {
    for (unsigned int i = list->size; i > index_start; i--) {
        list->data[i] = list->data[i - 1];
    }
}

// Shifts the elements of indices index_start .. size-1 one position to the left
void ptl_shift_left(t_prog_token_list* list, int index_start) {
    if (index_start > 0) {
        for (unsigned int i = index_start - 1; i < list->size - 1; i++) {
            list->data[i] = list->data[i + 1];
        }
    }
}

void ptl_push_front(t_prog_token_list *list, T_ptl val) {
    
    ptl_insert(list, 0, val);
}

void ptl_push_back(t_prog_token_list *list, T_ptl val) {

    if (list->size >= list->capacity) {
        ptl_realloc_list(list);
    }

    list->data[list->size] = val;
    list->size++;
}

T_ptl ptl_get(const t_prog_token_list *list, int index) {
    return list->data[index];
}

void ptl_set(t_prog_token_list *list, int index, T_ptl val) {
    list->data[index] = val;
}

void ptl_insert(t_prog_token_list *list, int index, T_ptl val) {
    if (list->size >= list->capacity) {
        ptl_realloc_list(list);
    }

    if (index == list->size) {
        ptl_push_back(list, val);
    }
    else {
        ptl_shift_right(list, index);
        list->data[index] = val;
        list->size++;
    }
}

void ptl_delete_at(t_prog_token_list *list, int index) {
    ptl_shift_left(list, index + 1);
    list->size--;
}

void ptl_print_list(const t_prog_token_list *list) {
    printf("[\n");
    for (int i = 0; i < list->size; i++) {
        print_prog_token(&list->data[i]);
        T_ptl t = list->data[i];
        e_prog_token_type type = t.token_type;
        if (type == PT_EXPR || (type == PT_KEYWORD && (t.content.keyword == KW_ELSE || t.content.keyword == KW_ENDBLOCK)))
            printf("\n");
        else
            printf(" ");
    }
    printf("]\n");
}

void ptl_destroy_list(t_prog_token_list *list) {
    // Do not destroy the expr_rpn in list->data[i], because the expr_rpn are copied into the ast
    // destroy_ast destroys them
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}