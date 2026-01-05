#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "text_renderer.h"

int text_renderer_init(TextRenderer *tr, const char *font_path, int font_size) {
	tr->font = TTF_OpenFont(font_path, font_size);
	if (tr->font == NULL)
		return 0;
	
	tr->color = (SDL_Color) {255, 255, 255, 255};
	return 1;
}

void text_renderer_destroy(TextRenderer *tr) {
	if (!tr->font) return;

	TTF_CloseFont(tr->font);
	tr->font = NULL;
}

SDL_Texture *text_render(TextRenderer *tr, SDL_Renderer *renderer, const char *text) {
	SDL_Surface *surface = TTF_RenderText_Blended(tr->font, text, tr->color);

	if (!surface) return NULL;

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
};
