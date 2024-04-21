#include "viewport.h"
#include "options.h"

Viewport *init_viewport(VectorD offset, VectorD size, double scale) {
	Viewport *viewport = malloc(sizeof(Viewport));

	double min_size;

	if (size.x < size.y) {
		min_size = size.x;
	} else {
		min_size = size.y;
	}

	viewport->scale = scale;
	viewport->conversion = min_size / scale;

	viewport->position = offset;
	viewport->size = size;
	viewport->offset.x = (double)size.x / 2 + offset.x;
	viewport->offset.y = (double)size.y / 2 + offset.y;

	return viewport;
}

void draw_viewport_grid(SDL_Renderer *renderer, Viewport *viewport) {
	double min_size = viewport->size.x < viewport->size.y ? viewport->size.x
														  : viewport->size.y;
	double cell_size = min_size / 10;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// vertical lines
	for (int i = 1; i < viewport->size.x / cell_size; i++) {
		SDL_RenderDrawLine(renderer,
						   i * cell_size + viewport->offset.x,
						   viewport->position.y,
						   i * cell_size + viewport->offset.x,
						   viewport->position.y + viewport->size.y);
	}

	// horizontal lines
	for (int i = 1; i < viewport->size.y / cell_size; i++) {
		SDL_RenderDrawLine(renderer,
						   viewport->position.x,
						   i * cell_size + viewport->offset.y,
						   viewport->position.x + viewport->size.x,
						   i * cell_size + viewport->offset.y);
	}
}

void handle_cursor(Viewport *viewport, SDL_Event *event) {
	int x, y, state;

	// get values
	if (event->type == SDL_MOUSEBUTTONUP ||
		event->type == SDL_MOUSEBUTTONDOWN) {
		x = event->button.x;
		y = event->button.y;
		state = event->button.state;
	} else if (event->type == SDL_MOUSEMOTION) {
		x = event->motion.x;
		y = event->motion.y;
		state = event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT);
	} else
		return;

	int type = SDL_SYSTEM_CURSOR_ARROW;

	if (state == SDL_PRESSED)
		type = SDL_SYSTEM_CURSOR_HAND;
	else if (state == SDL_RELEASED)
		type = SDL_SYSTEM_CURSOR_ARROW;

	// if outside viewport, normal cursor
	if (x < viewport->position.x ||
		x > viewport->position.x + viewport->size.x ||
		y < viewport->position.y ||
		y > viewport->position.y + viewport->size.y) {
		type = SDL_SYSTEM_CURSOR_ARROW;
	}

	// not sure if we need to free these?
	SDL_Cursor *cursor = SDL_CreateSystemCursor(type);
	SDL_SetCursor(cursor);
}

void handle_pan(Viewport *viewport, SDL_Event *event) {
	if (event->type != SDL_MOUSEMOTION) return;

	SDL_MouseMotionEvent motion = event->motion;

	if ((motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) == 0) return;

	if (motion.x < viewport->position.x ||
		motion.x > viewport->position.x + viewport->size.x ||
		motion.y < viewport->position.y ||
		motion.y > viewport->position.y + viewport->size.y)
		return;

	viewport->offset.x += motion.xrel;
	viewport->offset.y += motion.yrel;
}

void handle_zoom(Viewport *viewport, SDL_Event *event) {
	if (event->type != SDL_MOUSEWHEEL) return;

	// https://wiki.libsdl.org/SDL2/SDL_MouseWheelEvent
	SDL_MouseWheelEvent wheel = event->wheel;

	if (wheel.mouseX < viewport->position.x ||
		wheel.mouseX > viewport->position.x + viewport->size.x ||
		wheel.mouseY < viewport->position.y ||
		wheel.mouseY > viewport->position.y + viewport->size.y)
		return;

	double min_size = viewport->size.x < viewport->size.y ? viewport->size.x
														  : viewport->size.y;

	// always stepping 1/10th of the distance to 0
	// smoothly changes zoom distance with scale
	viewport->scale += -wheel.y * viewport->scale / 10;

	// when scale < 0, then viewport inverted
	// we don't want inverted, also no divide by 0
	if (viewport->scale <= 0) viewport->scale = 1e-5;

	viewport->conversion = min_size / viewport->scale;
}

void handle_viewport_nav(Viewport *viewport, SDL_Event *event) {
	handle_cursor(viewport, event);
	handle_pan(viewport, event);
	handle_zoom(viewport, event);
}
