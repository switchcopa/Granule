// engine.h
#ifndef ENGINE_H
#define ENGINE_H

#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS)
#define DT (1.0f / TARGET_FPS)

typedef struct {
	int running;
} Engine;

int 	engine_init(Engine *engine);
void 	engine_run(Engine *engine);
void 	engine_shutdown(Engine *engine);

#endif
