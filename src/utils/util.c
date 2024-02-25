#include "util.h"

#include <string.h>
#include <stdlib.h>

char* create_string(const char* str) {
	size_t length = strlen(str);
	char* new_str = malloc(sizeof(char) * length);
	strcpy(new_str, str);
	return new_str;
}
