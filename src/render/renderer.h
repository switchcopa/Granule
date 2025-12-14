// renderer.h

#include "../world/world.h"

#ifndef RENDERER_H
#define RENDERER_H

#define SAND_SIZE 4

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS)

typedef enum {
	NONE,
	YELLOW,
	BLUE
} CellColor;

int renderer_init(void);
void renderer_begin(void);
void renderer_draw(World *world);
void renderer_end(void);
void renderer_shutdown(void);

#endif
