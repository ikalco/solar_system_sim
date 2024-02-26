#include "linked_list.h"
#include <stdlib.h>

List *create_list() {
	List *new_list = (List *)malloc(sizeof(List));
	new_list->size = 0;
	new_list->first = NULL;
	return new_list;
}

void add_list(List *list, void *data) {
	Node *new_node = (Node *)malloc(sizeof(Node));
	new_node->data = data;
	new_node->next = list->first;
	list->first = new_node;
	list->size++;
}

void *remove_list(List *list) {
	if (list->size == 0) {
		return NULL;
	}

	Node *removed = list->first;
	void *data = removed->data;

	list->first = removed->next;
	list->size--;

	free(removed);

	return data;
}

void free_list(List *list) {
	Node *current = list->first;
	while (current != NULL) {
		Node *next = current->next;

		if (current->data != NULL) {
			free(current->data);
		}

		free(current);

		current = next;
	}

	free(list);
}
