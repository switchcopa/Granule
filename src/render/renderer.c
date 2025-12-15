#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "renderer.h"
#include "../world/world.h"

static ColorType get_grid_color(World *world, int i, int j);

SDL_Window *window;
SDL_Renderer *renderer;

int renderer_init(void) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return 0;

	window = SDL_CreateWindow("Granule", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_ShowCursor(SDL_DISABLE);
        return 1;
}

void renderer_begin(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void renderer_draw(World *world) {
	for (int i = 0; i < world->height; i++)
		for (int j = 0; j < world->width; j++) {
			switch (get_grid_color(world, i, j)) {
				case NONE:
					break;
				case YELLOW:
					SDL_Rect sand = {j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
					SDL_RenderFillRect(renderer, &sand);
					break;
				case BLUE:
					SDL_Rect water = {j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
					SDL_RenderFillRect(renderer, &water);
					break;
				case BROWNISH_YELLOW:
					SDL_Rect wet_sand = {j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
					SDL_RenderFillRect(renderer, &wet_sand);
					break;
			}
		} 
}

void renderer_end(void) {
	SDL_RenderPresent(renderer);
}

void renderer_shutdown(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void renderer_draw_cursor(World *world, int mx, int my) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect cursor = {mx, my, BLOCK_SIZE * world->generation_size, BLOCK_SIZE * world->generation_size};
	SDL_RenderDrawRect(renderer, &cursor);
}

static ColorType get_grid_color(World *world, int i, int j) {
	switch (world->grid[i][j]) {
		case EMPTY:
			return NONE;
		case SAND:
			SDL_SetRenderDrawColor(renderer, world->color_buffer[i][j].r, 
                                world->color_buffer[i][j].g, world->color_buffer[i][j].b, 255);
			return YELLOW;
		case WET_SAND:
			SDL_SetRenderDrawColor(renderer, world->color_buffer[i][j].r, 
                                world->color_buffer[i][j].g, world->color_buffer[i][j].b, 255);
			return BROWNISH_YELLOW;
		case WATER:
			SDL_SetRenderDrawColor(renderer, world->color_buffer[i][j].r, 
                                world->color_buffer[i][j].g, world->color_buffer[i][j].b, 255);
			return BLUE;
		default:
			return NONE;
	}
}
