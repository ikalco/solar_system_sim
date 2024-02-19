#ifndef SAVE_H
#define SAVE_H

#include "linked_list.h"
#include "bodies.h"

#define MAX_SAVE_LINE_LEN 255

List* read_save_file(char* filename);
void write_save_file(char* filename, List* bodies);

#endif
