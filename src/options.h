#ifndef OPTIONS_H
#define OPTIONS_H

#include "engine/physical.h"

// window options
#define WINDOW_TITLE "Solar System Simulator"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_FPS 60

// viewport options
#define VIEWPORT_SIZE 3.5
#define VIEWPORT_UNIT AU

// save file options
#define DEFAULT_SAVE_FILE "saves/solar_system.save"
#define MAX_SAVE_LINE_LEN 255

// physics engine options
#define TIME_STEP 60 * 60 * 24 // in seconds, this is one day btw

#endif
