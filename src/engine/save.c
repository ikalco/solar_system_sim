#include "save.h"
#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// in format of
// position.x,position.y,velocity.x,velocity.y,mass,color.red,color.green,color.blue,name
// first 5 are doubles
// next are 3 uint8_t
// final is a string (char*) that goes until end of line
PhysicalBody *parse_save_line(char *line) {
	PhysicalBody *body = malloc(sizeof(PhysicalBody));
	body->name = NULL;

	const char delim[] = ",";

	char *token = strtok(line, delim);
	if (token == NULL) return NULL;
	sscanf(token, "%lf", &body->position.x);

	token = strtok(NULL, delim);
	if (token == NULL) return NULL;
	sscanf(token, "%lf", &body->position.y);

	token = strtok(NULL, delim);
	if (token == NULL) return NULL;
	sscanf(token, "%lf", &body->velocity.x);

	token = strtok(NULL, delim);
	if (token == NULL) return NULL;
	sscanf(token, "%lf", &body->velocity.y);

	token = strtok(NULL, delim);
	if (token == NULL) return NULL;
	sscanf(token, "%lf", &body->mass);

	token = strtok(NULL, delim);
	if (token == NULL) return NULL;
	sscanf(token, "%hhu", &body->color.red);

	token = strtok(NULL, delim);
	if (token == NULL) return NULL;
	sscanf(token, "%hhu", &body->color.green);

	token = strtok(NULL, delim);
	if (token == NULL) return NULL;
	sscanf(token, "%hhu", &body->color.blue);

	// alpha of color for all bodies is alway 255
	body->color.alpha = 255;

	token = strtok(NULL, delim);
	if (token == NULL) return NULL;

	int length = strlen(token);

	if (token[length - 1] == '\n') {
		token[length - 1] = '\0';
		length--;
	}

	body->name = malloc(sizeof(char) * length);
	strcpy(body->name, token);

	return body;
}

List *read_save_file(char *filename) {
	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Failed to open save file for reading: %s", filename);
		exit(1);
	}

	char buf[MAX_SAVE_LINE_LEN];
	int line_number = 1;
	List *bodies = malloc(sizeof(List));

	while (fgets(buf, MAX_SAVE_LINE_LEN, file) != NULL) {
		PhysicalBody *body = parse_save_line(buf);

		if (body == NULL) {
			free_body(body);
			printf("Invalid line in save file: %s at line %d", filename, line_number);
			// yes i'm using line_number as a failure flag
			line_number = -1;
			break;
		}

		print_phyiscal_body(body);

		add_list(bodies, body);
		line_number++;
	}

	if (fclose(file) == EOF) {
		printf("Failed to close save file: %s", filename);
		free_bodies(bodies);
		exit(1);
	}

	if (line_number == -1) {
		free_bodies(bodies);
		exit(1);
	}

	return bodies;
}

void write_save_file(char *filename, List *bodies) {
	FILE *file = fopen(filename, "wb");
	if (file == NULL) {
		printf("Failed to open save file for writing: %s", filename);
		exit(1);
	}

	char line[MAX_SAVE_LINE_LEN];

	for (Node *current_node = bodies->first; current_node != NULL;
		 current_node = current_node->next) {
		PhysicalBody *body = current_node->data;

		// using same char buffer size to ensure read_save_file will function properly when this
		// save file is read
		int res = snprintf(
			line,
			MAX_SAVE_LINE_LEN,
			"%lf,%lf,%lf,%lf,%lf,%hhu,%hhu,%hhu,%s\n",
			body->position.x,
			body->position.y,
			body->velocity.x,
			body->velocity.y,
			body->mass,
			body->color.red,
			body->color.green,
			body->color.blue,
			body->name
		);

		if (res < 0) {
			printf("Failed to write save file: %s", filename);
			break;
		}

		if (res >= MAX_SAVE_LINE_LEN) {
			printf("Failed to write save file because the line was too long: %s", filename);
			break;
		}

		res = fputs(line, file);

		if (res == EOF) {
			printf("Failed to write save file: %s", filename);
			break;
		}
	}

	if (fclose(file) == EOF) {
		printf("Failed to close save file: %s", filename);
		exit(1);
	}
}
