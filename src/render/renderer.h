// renderer.h

#include "../world/world.h"

#ifndef RENDERER_H
#define RENDERER_H

#define BLOCK_SIZE 4
#define GAS_ALPHA 50

#define WINDOW_WIDTH 1600 
#define WINDOW_HEIGHT 900 
#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS)

int renderer_init(void);
void renderer_begin(void);
void renderer_draw(World *world);
void renderer_draw_text(const char *text, int w, int h);
void renderer_draw_cursor(World *world, int mx, int my);
void renderer_end(void);
void renderer_shutdown(void);

#endif
