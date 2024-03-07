#include "menu_util.h"
#include <stdbool.h>

bool inside_node(MenuNode *node, int mouse_x, int mouse_y) {
	return (
		mouse_x > node->render_pos.x && mouse_y > node->render_pos.y &&
		mouse_x < node->render_pos.x + node->size.x &&
		mouse_y < node->render_pos.y + node->size.y
	);
}

int find_mouse_menu_node(MenuNode *node, int mouse_x, int mouse_y) {
	if (!inside_node(node, mouse_x, mouse_y)) return -1;

	if (node->type != MENU_LIST) return node->id;

	// find which list element was clicked
	MenuList *list = node->node;

	MenuNode *next = list->child;
	int res = -1;

	double spacing_offset = 0;

	while (next != NULL) {
		if (list->direction == MENU_VERTICAL) {
			next->offset.y += spacing_offset;
		} else if (list->direction == MENU_HORIZONTAL) {
			next->offset.x += spacing_offset;
		}

		res = find_mouse_menu_node(next, mouse_x, mouse_y);

		if (list->direction == MENU_VERTICAL) {
			next->offset.y -= spacing_offset;
		} else if (list->direction == MENU_HORIZONTAL) {
			next->offset.x -= spacing_offset;
		}

		if (res != -1) break;

		if (list->direction == MENU_VERTICAL) {
			spacing_offset += list->spacing + next->offset.y + next->size.y;
		} else if (list->direction == MENU_HORIZONTAL) {
			spacing_offset += list->spacing + next->offset.x + next->size.x;
		}

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

MenuNode *find_menu_node_id(MenuNode *node, int id) {
	if (node->id == id) return node;
	if (node->type != MENU_LIST) return NULL;

	MenuList *list = node->node;

	MenuNode *next = list->child;
	MenuNode *res = NULL;

	while (next != NULL) {
		res = find_menu_node_id(next, id);

		if (res != NULL) return res;

		next = next->next;
	}

	return NULL;
}

int menu_has_clicked(MenuRoot *root, SDL_Event *event) {
	if (event->type == SDL_MOUSEBUTTONDOWN) {
		root->prev_clicked_id =
			find_mouse_menu_root(root, event->button.x, event->button.y);
	} else if (event->type == SDL_MOUSEBUTTONUP) {
		int new_clicked_id =
			find_mouse_menu_root(root, event->button.x, event->button.y);

		// clicked and released in same menu node
		if (root->prev_clicked_id == new_clicked_id) {
			root->prev_clicked_id = -1;
			return new_clicked_id;
		}
	}

	return -1;
}
