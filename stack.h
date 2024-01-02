#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdbool.h>

typedef struct node {
    uint16_t element;
    struct node *next;
} node;


typedef struct {
    uint64_t size;
    node *head;
} stack;

void push(stack *s, uint16_t e);
uint16_t pop(stack *s);
bool isEmpty(stack *s);
void freeStack(stack *s);

#endif
