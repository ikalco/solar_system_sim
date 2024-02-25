#ifndef INCLUDES_H
#define INCLUDES_H

#include "options.h"

#include "engine/physical.h"
#include "engine/bodies.h"
#include "engine/viewport.h"
#include "engine/save.h"

#include "utils/util.h"
#include "utils/linked_list.h"
#include "menu/menu.h"

void initSDL();
void cleanup();
void handle_input();

#endif
