#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include "list_double-ended.h"

typedef struct {
    t_list list;
} t_queue;

t_queue create_empty_queue();

// Returns true if the queue is empty
bool is_empty_queue(const t_queue *queue);

// Pushes the value val to the back of the queue
void push_queue(t_queue *queue, T val);

// Returns the value at the front of the queue
T get_front_queue(const t_queue *queue);

// Returns the value at the front of the queue and deletes it from the queue
T pop_queue(t_queue *queue);

// Destroys the queue
void destroy_queue(t_queue *queue);

// Prints the content of the queue
void print_queue(const t_queue *queue);

#endif