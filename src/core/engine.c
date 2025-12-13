#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "engine.h"
#include <stdlib.h>
#include <stdio.h>
#include "../render/renderer.h"
#include "../world/world.h"

World world;

int engine_init(Engine *engine) {
	engine->running = 0;
	world_init(&world);
	return renderer_init() == 1;
}

void engine_run(Engine *engine) {
	SDL_Event event;
	engine->running = 1;
		
	while (engine->running) {
		Uint32 start_time = SDL_GetTicks();
		while (SDL_PollEvent(&event)) 
			switch (event.type) {
				case SDL_QUIT:
					engine->running = 0;
					break;	
			}
	
		world_update(&world, DT);

		renderer_begin();
		renderer_draw();
		renderer_end();	

		Uint32 frame_time = SDL_GetTicks() - start_time;
		if (frame_time < TARGET_FPS)
			SDL_Delay(FRAME_TIME - frame_time); 
	}
}

void engine_shutdown(Engine *engine) {
	world_destroy(&world);
	renderer_shutdown();
}
