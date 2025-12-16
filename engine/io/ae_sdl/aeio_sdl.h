// sdl_output.h
//
// IO module for outputting buffer to an sdl2 window

#ifndef AEIO_SDL_H
#define AEIO_SDL_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "../buffer.h"

typedef struct AEIO_SDL_Context
{
	int res_x, res_y;
	int scale;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
} AEIO_SDL_Context;


AEIO_SDL_Context aeio_sdl_init(const char *window_title, int res_x, int res_y, int scale);
void aeio_sdl_destroy(AEIO_SDL_Context *context);
void aeio_sdl_set_relative_mouse_mode(bool enabled);

void aeio_sdl_draw(AEIO_SDL_Context *context, const Buffer *buffer);

#endif // AEIO_SDL_H
