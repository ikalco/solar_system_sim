#include "menu_util.h"
#include <stdbool.h>

bool inside_node(MenuNode *node, int mouse_x, int mouse_y) {
	return (
		mouse_x > node->offset.x && mouse_y > node->offset.y &&
		mouse_x < node->offset.x + node->size.x &&
		mouse_y < node->offset.y + node->size.y
	);
}

int find_mouse_menu_node(MenuNode *node, int mouse_x, int mouse_y) {
	if (!inside_node(node, mouse_x, mouse_y)) return -1;

	if (node->type != MENU_LIST) return node->id;

	// find which list element was clicked
	MenuVerticalList *list = node->node;

	MenuNode *next = list->child;
	int res = -1;

	while (next != NULL) {
		res = find_mouse_menu_node(next, mouse_x, mouse_y);

		if (res != -1) break;

		next = next->next;
	}

	// if its still -1 that means it's not inside any of our sub elements
	// meaning the list itself was clicked
	if (res == -1) return node->id;

	return res;
}

int find_mouse_menu_root(MenuRoot *root, int mouse_x, int mouse_y) {
	MenuNode *root_node = root->root;

	// offset mouse position by menu position,
	// since internal menu offsets are from root
	int offset_x = mouse_x - root->position.x;
	int offset_y = mouse_y - root->position.y;

	return find_mouse_menu_node(root_node, offset_x, offset_y);
}
