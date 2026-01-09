#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "renderer.h"
#include "../world/world.h"
#include "../../include/shared/colors.h"
#include "text_renderer.h"

SDL_Window *window;
SDL_Renderer *renderer;
TextRenderer tr;

const char *font_path = "assets/Pixelify_Sans/Pixelify_Sans.ttf";

static void set_cell_color(World *world, int i, int j);

int renderer_init(void) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() == -1)
		return 0;

	window = SDL_CreateWindow("Granule", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (text_renderer_init(&tr, font_path, FONT_SIZE) == 0) {
		printf("error: failed to load font '%s\n", TTF_GetError());
		return 0;
	}

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
                        set_cell_color(world, i, j);
                        SDL_Rect block = {j*BLOCK_SIZE, i*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                        SDL_RenderFillRect(renderer, &block);
		}
} 

void renderer_draw_text(const char *text, int w, int h) {
	SDL_Texture *tex = text_render(&tr, renderer, text);
	if (!tex) return;

	int W, H;
	SDL_QueryTexture(tex, NULL, NULL, &W, &H);
	SDL_Rect r = {w, h, W, H};
	
	SDL_RenderCopy(renderer, tex, NULL, &r);
	SDL_DestroyTexture(tex);
}

void renderer_end(void) {
	SDL_RenderPresent(renderer);
}

void renderer_shutdown(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();
}

void renderer_draw_cursor(World *world, int mx, int my) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect cursor = {mx, my, BLOCK_SIZE * world->generation_size, BLOCK_SIZE * world->generation_size};
	SDL_RenderDrawRect(renderer, &cursor);
}

static void set_cell_color(World *world, int i, int j) {
	Cell cell = world->grid[i][j];
        uint8_t r = cell.color.r;
        uint8_t g = cell.color.g;
        uint8_t b = cell.color.b;
	uint8_t a = (cell.state == GAS) ? 70 : 255;

        SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
