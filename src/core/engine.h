// engine.h

#include "../world/world.h"

#ifndef ENGINE_H
#define ENGINE_H

#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS)
#define DT (1.0f / TARGET_FPS)
#define MODES_N 2

typedef enum {
	NONE,
	ERASE,
} Mode;

typedef struct {
        Cell *hovered_cell;
	CellType palette;
	Mode mode;
	int running;
	int cursor_x, cursor_y;
} Engine;

int 	engine_init(Engine *engine);
void 	engine_run(Engine *engine);
void 	engine_shutdown(void);

#endif
