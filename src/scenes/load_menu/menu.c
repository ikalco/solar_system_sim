#include "menu.h"

#include "menu/menu.h"
#include "options.h"
#include "utils/util.h"

#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>

bool valid_save_file_name(char *save_file_name) {
	char save_file_path[512];
	struct stat save_file_info;

	sprintf(save_file_path, "%s/%s", DEFAULT_SAVE_PATH, save_file_name);
	if (stat(save_file_path, &save_file_info) < 0) {
		// failed to stat file
		printf("Failed to read \"%s\" when reading saves.\n", save_file_name);
		return 0;
	}

	// if it's a directory then don't add it to the list
	if (S_ISDIR(save_file_info.st_mode)) {
		return 0;
	}

	// if the file name is less than 6 characters or does't end in ".save"
	int save_file_len = strlen(save_file_name);
	if (save_file_len < 6 ||
		strcmp(save_file_name + save_file_len - 5, ".save")) {
		return 0;
	}

	// null terminate at the just before the ".save", so that only save name
	// is displayed
	save_file_name[save_file_len - 5] = 0;

	return 1;
}

int add_saves_to_list(MenuNode *save_list_node, MenuVerticalList *save_list) {
	struct dirent *save_file;
	DIR *directory;

	directory = opendir(DEFAULT_SAVE_PATH);
	if (directory == NULL) {
		printf("Failed to open saves folder: %s", DEFAULT_SAVE_PATH);
		exit(1);
	}

	// have individual menu id's for each save file button
	int save_file_id = SAVE_BUTTONS;

	// read files from directory stream
	// we aren't checking for duplicates so hopefully that doesn't happen
	while ((save_file = readdir(directory))) {
		if (!valid_save_file_name(save_file->d_name)) continue;

		MenuButton *save_file_button = create_menu_button(
			(Color){130, 130, 130, 255},
			(Color){235, 235, 235, 255},
			TEXT_CENTER,
			create_string(save_file->d_name)
		);

		MenuNode *save_file_node = create_menu_node(
			save_file_id,
			(VectorD){0, 0},
			(VectorD){MENU_MAX_SIZE, 60},
			save_list_node,
			MENU_BUTTON,
			save_file_button
		);

		add_menu_vlist(save_list, save_file_node);

		save_file_id++;
	}

	closedir(directory);

	return save_file_id - SAVE_BUTTONS;
}

MenuNode *init_saves_list(MenuNode *root_node, Data *data) {
	MenuVerticalList *save_list =
		create_menu_vlist((Color){50, 50, 50, 240}, (VectorD){5, 5}, 10);
	MenuNode *save_list_node = create_menu_node(
		SAVE_LIST,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 0},
		root_node,
		MENU_LIST,
		save_list
	);

	// returns numbers of saves added to list, so we save it
	data->num_saves = add_saves_to_list(save_list_node, save_list);

	return save_list_node;
}

MenuRoot *init_menu_root(SDL_Window *window, Data *data) {
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList *root_list =
		create_menu_vlist((Color){40, 40, 40, 255}, (VectorD){20, 20}, 10);
	MenuNode *root_node = create_menu_node(
		ROOT_LIST,
		(VectorD){10, 10},
		(VectorD){260, 460},
		NULL,
		MENU_LIST,
		root_list
	);

	MenuText *title_text =
		create_menu_text(text_color, TEXT_CENTER, create_string("Saves"));
	MenuNode *title_text_node = create_menu_node(
		TITLE_TEXT,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 30},
		root_node,
		MENU_TEXT,
		title_text
	);
	add_menu_vlist(root_list, title_text_node);

	MenuNode *saves_list_node = init_saves_list(root_node, data);
	add_menu_vlist(root_list, saves_list_node);

	MenuRoot *root = create_menu_root(
		window,
		(VectorD){100, 100},
		(VectorD){300, 500},
		"fonts/Roboto-Regular.ttf",
		root_node
	);

	render_menu_root(root);

	return root;
}
