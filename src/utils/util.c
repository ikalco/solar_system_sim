#include "util.h"

#include <stdlib.h>
#include <string.h>

char *create_string(const char *str) {
	size_t length = strlen(str);
	char *new_str = malloc(sizeof(char) * length);
	strcpy(new_str, str);
	return new_str;
}
