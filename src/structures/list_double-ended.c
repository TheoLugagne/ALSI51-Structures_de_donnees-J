#include <stdio.h>
#include <stdlib.h>

#include "structures/list_double-ended.h"

t_list create_empty_list() {
    t_list l;
    l.head = NULL;
    l.tail = NULL;
    l.size = 0;
    return l;
}

int length(const t_list *list) {
    return list->size;
}

t_cell *create_cell(T val) {
    t_cell *newcell = (t_cell*) malloc(sizeof(t_cell));
    newcell->value = val;
    newcell->next = NULL;
    return newcell;
}

// O(1)
void push_front(t_list *list, T val) {
    
    t_cell *newcell = create_cell(val);
    newcell->next = list->head;
    list->head = newcell;
    list->size++;
    if (list->tail == NULL) {
        list->tail = newcell;
    }
}

t_cell* get_cell(const t_list *list, int index) {
    t_cell *cell = list->head;
    if (cell == NULL || index < 0 || index > list->size)
        return NULL;
    
    if (index == 0) {
        return cell;
    }
    
    if (index == list->size - 1 && list->size != 0) {
        return list->tail;
    }

    int i = 0;
    while (!(i == index || cell == NULL)) {
        cell = cell->next;
        i++;
    }
    return cell;
}

// O(1)
t_cell* get_last(const t_list *list) {
    return get_cell(list, list->size - 1);
}

// O(1) (better than O(n) for the lists)
void push_back(t_list *list, T val) {

    if (list->size == 0) {
        push_front(list, val);
        return;
    }
    t_cell *newcell = create_cell(val);
    t_cell *lastcell = get_last(list);
    lastcell->next = newcell;
    list->tail = newcell;
    list->size++;
}

T get(const t_list *list, int index) {
    t_cell *cell = get_cell(list, index);
    return cell->value;
}

void set(t_list *list, int index, T val) {
    t_cell *cell = get_cell(list, index);
    cell->value = val;
}

void insert(t_list *list, int index, T val) {
    if (index == 0) {
        push_front(list, val);
        return;
    }
    else if (index == list->size - 1 && list->size != 0) {
        push_back(list, val);
        return;
    }

    t_cell *prev = get_cell(list, index - 1);
    t_cell *newcell = create_cell(val);
    newcell->next = prev->next;
    prev->next = newcell;
    list->size++;
}

// O(1) for head
// O(n) worst case
void delete_at(t_list *list, int index) {
    if (index == 0) {
        t_cell *first = list->head;
        if (first == NULL)
            return;
        list->head = first->next;
        free(first);
        list->size--;
        if (list->size == 0)
            list->tail = NULL;
        return;
    }

    t_cell *prev = get_cell(list, index - 1);
    t_cell *cell = prev->next;
    if (prev == NULL || cell == NULL)
        return;

    t_cell *next = cell->next;
    free(cell);
    prev->next = next;
    list->size--;
    
    if (index == list->size) {
        list->tail = prev;
    }
}

void print_list(const t_list *list) {
    
    t_cell *cell = list->head;
    while(cell != NULL) {
        print_token(&cell->value);
        cell = cell->next;
        if (cell) printf(" ");
    }
}

void print_list_file(FILE *file, const t_list *list) {
    
    t_cell *cell = list->head;
    while(cell != NULL) {
        print_token_file(file, &cell->value);
        cell = cell->next;
        if (cell) fprintf(file, " ");
    }
}

void destroy_list(t_list *list) {

    t_cell *cell = list->head;
    while (cell != NULL) {
        t_cell *next = cell->next;
        free(cell);
        cell = next;
    }
    list->head = NULL;
    list->tail = NULL;
}