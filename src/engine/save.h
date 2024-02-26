#ifndef SAVE_H
#define SAVE_H

#include "bodies.h"
#include "utils/linked_list.h"

List *read_save_file(char *filename);
void write_save_file(char *filename, List *bodies);

#endif
