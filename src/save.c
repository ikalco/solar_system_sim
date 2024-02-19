#include "save.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

PhysicalBody* parse_save_line(char* line) {
	PhysicalBody* body = malloc(sizeof(PhysicalBody));

	const char delim[] = ",";

	char* token = strtok(line, delim);
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

List* read_save_file(char* filename) {
	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Failed to open save file for reading: %s", filename);
		exit(1);
	}

	char buf[255];
	int line_number = 1;
	List* bodies = malloc(sizeof(List));
	
	while (fgets(buf, 255, file) != NULL) {
		PhysicalBody* body = parse_save_line(buf);

		if (body == NULL) {
			printf("Invalid line (%d) in save file: %s", line_number, filename);
			exit(1);
		}

		print_phyiscal_body(body);

		add_list(bodies, body);
		line_number++;
	}

	if (fclose(file) == EOF) {
		printf("Failed to close save file: %s", filename);
		exit(1);
	}

	return bodies;
}

void write_save_file(char* filename, List* bodies) {
	FILE* file = fopen(filename, "wb");
	if (file == NULL) {
		printf("Failed to open save file for writing: %s", filename);
		exit(1);
	}

	for (Node* current_node = bodies->first; current_node != NULL; current_node = current_node->next) {
		PhysicalBody* body = current_node->data;

		fprintf(file, "%lf,%lf,%lf,%lf,%lf,%hhu,%hhu,%hhu,%s\n",
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
	}

	if (fclose(file) == EOF) {
		printf("Failed to close save file: %s", filename);
		exit(1);
	}
}
