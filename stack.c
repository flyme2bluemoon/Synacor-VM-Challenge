// defines a stack using a linked list
#include "stack.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void push(stack *s, uint16_t e) {
    node *n = malloc(sizeof(node));
    n->element = e;
    if (s->size == 0) {
        n->next = NULL;
        s->head = n;
    } else {
        n->next = s->head;
        s->head = n;
    }
    s->size++;
}

uint16_t pop(stack *s) {
    if (s->size == 0) {
        fprintf(stderr, "Cannot pop from empty stack\n");
        exit(1);
    }

    node *old_head = s->head;
    uint16_t e = old_head->element;

    s->head = old_head->next;

    free(old_head);

    s->size--;

    return e;
}

bool isEmpty(stack *s) {
    return s->size == 0;
}

void freeStack(stack *s) {
    while (!isEmpty(s)) {
        pop(s);
    }
}
