#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "renderer.h"

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
        return 1;
}

void renderer_begin(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void renderer_draw(void) {

}

void renderer_end(void) {
	SDL_RenderPresent(renderer);
}

void renderer_shutdown(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
