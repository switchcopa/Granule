
// text_renderer.h
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define FONT_SIZE 28 

typedef struct {
	TTF_Font *font;
	SDL_Color color;
} TextRenderer;

int text_renderer_init(TextRenderer *tr, const char *font_path, int font_size);
void text_renderer_destroy(TextRenderer *tr);
SDL_Texture *text_render(TextRenderer *tr, SDL_Renderer *renderer, const char *text);

#endif 
