#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

typedef struct node {
	void* data;
	struct node* next;
} Node;

typedef struct list {
	Node* first;
	int size;
} List;

List* create_list();
void add_list(List* list, void* data);
void* remove_list(List* list);
void free_list(List* list);

#endif
