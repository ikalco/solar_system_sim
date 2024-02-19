#ifndef INCLUDES_H
#define INCLUDES_H

#include "physical.h"
#include "options.h"

#include "structs.h"
#include "linked_list.h"
#include "bodies.h"
#include "viewport.h"
#include "save.h"

void initSDL();
void cleanup();
void handle_input();

#endif
