#ifndef LIST_H
#define LIST_H

#include <stdlib.h>


typedef struct element element;
typedef struct list list;

// These type definitions could also be in list.c.
// Having it here, allows full access from main.c, which could help with debugging.
struct element {
	int payload;
	element *next;
};

typedef struct list {
	element *head;
	element **tail;
} list;

void enqueue(list *list, element *item);
list* initialize_list();

// uncomment these (and implement them in list.c):
void enqueue_front(list *list, element* elem);
element* dequeue(list *list);
void delete_elements(list *list, int value);

#endif
