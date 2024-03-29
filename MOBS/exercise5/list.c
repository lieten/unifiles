#include "list.h"
#include <stdio.h>
// Diese Datei soll abegegeben werden


void enqueue(list *list, element *item) {
	item->next = NULL;
	*list->tail = item;
	list->tail = &item->next;
}

list* initialize_list() {
	list *new_list = (list*) malloc(sizeof(list));
	new_list->head = NULL;
	new_list->tail = &new_list->head;
	return new_list;
}

void enqueue_front(list *list, element* elem) {
	element *prevHead = list->head;
	list->head = elem;
	elem->next = prevHead;
}

element* dequeue(list *list) {
	element *prevHead = list->head;
	if(prevHead != NULL) {
		list->head = prevHead->next;
		prevHead->next = NULL;
	}
	return prevHead;
}

void delete_elements(list *list, int value) {
	element *currentElement = list->head;
	element *previousElement = NULL;
	while(currentElement != NULL) {
		int elemVal = currentElement->payload;
		if(elemVal == value) {
			if(previousElement == NULL) {
				list->head = currentElement->next;
				currentElement->next = NULL;
				free(currentElement);
				currentElement = list->head;
			}
			else {
				previousElement->next = currentElement->next;
				currentElement->next = NULL;
				free(currentElement);
				currentElement = previousElement->next;
			}
		}
		else {
			previousElement = currentElement;
			currentElement = currentElement->next;
		}
	}
}
