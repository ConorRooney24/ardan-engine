// aeio_sdl.c

#include "aeio_sdl.h"
#include "../../col.h"
#include "../buffer.h"

AEIO_SDL_Context aeio_sdl_init(const char *window_title, int res_x, int res_y, int scale)
{
	AEIO_SDL_Context context;
	context.res_x = res_x;
	context.res_y = res_y;
	context.scale = scale > 0 ? scale : 1;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		col_print_set(COL_RED);
		printf("Error: Unable to initialise SDL '%s'", SDL_GetError());
		col_print_reset();
		printf("\n");

		return context;
	}

	int window_width = res_x * context.scale;
	int window_height = res_y * context.scale;

	context.window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
	context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_ACCELERATED);
	context.texture = SDL_CreateTexture(context.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, res_x, res_y);

	return context;
}
void aeio_sdl_destroy(AEIO_SDL_Context *context)
{
	SDL_DestroyTexture(context->texture);
	SDL_DestroyRenderer(context->renderer);
	SDL_DestroyWindow(context->window);
	SDL_Quit();

	context->res_x = 0;
	context->res_y = 0;
}

void aeio_sdl_set_relative_mouse_mode(bool enabled)
{
	SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
}

void aeio_sdl_draw(AEIO_SDL_Context *context, const Buffer *buffer)
{
	if (context->res_x != buffer->res_x || context->res_y != buffer->res_y)
	{
		col_print_set(COL_RED);
		printf("Error: Buffer resolution does not match AEIO SDL Context resolution");
		col_print_reset();
		printf("\n");

		return;
	}

	void *texture_pixels = NULL;
	int pitch = 0;

	if (SDL_LockTexture(context->texture, NULL, &texture_pixels, &pitch) != 0)
	{
		col_print_set(COL_RED);
		printf("Error: Unable to lock SDL texture '%s'", SDL_GetError());
		col_print_reset();
		printf("\n");
		return;
	}

	for (int y = 0; y < buffer->res_y; y++)
	{
		uint32_t *row = (uint32_t *)((uint8_t *)texture_pixels + y * pitch);

		for (int x = 0; x < buffer->res_x; x++)
		{
			Col c = buffer->pixels[y][x];
			row[x] = (0xFFu << 24) | (c.r << 16) | (c.g << 8) | c.b;
		}
	}

	SDL_UnlockTexture(context->texture);

	SDL_RenderClear(context->renderer);
	SDL_Rect dest = {0, 0, context->res_x * context->scale, context->res_y * context->scale};
	SDL_RenderCopy(context->renderer, context->texture, NULL, &dest);
	SDL_RenderPresent(context->renderer);
}
