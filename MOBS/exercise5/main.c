#include <stdio.h>
#include "list.h"

void dq(list *l) {
	element *e = dequeue(l);
	if(e != NULL) {
		printf("%d\n", e->payload);
	}
	else {
		printf("Underflow\n");
	}
}

int main(void) {
	list *list = initialize_list();
	element *elem = (element*) malloc(sizeof(element));
	elem->payload = 100;
	enqueue(list, elem);
	element *elem2 = (element*) malloc(sizeof(element));
	elem2->payload = 200;
	enqueue(list, elem2);
	element *elem3 = (element*) malloc(sizeof(element));
	elem3->payload = 100;
	enqueue(list, elem3);
	element *elem4 = (element*) malloc(sizeof(element));
	elem4->payload = 300;
	enqueue(list, elem4);
	element *elem5 = (element*) malloc(sizeof(element));
	elem5->payload = 100;
	enqueue(list, elem5);

	delete_elements(list, 100);

	dq(list);
	dq(list);
	dq(list);
	//dq(list);
}
