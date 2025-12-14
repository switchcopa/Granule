#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "engine.h"
#include <stdlib.h>
#include <stdio.h> 
#include "../render/renderer.h"
#include "../world/world.h"


static void generate_sand(World *world, int x, int y);

World world;

int engine_init(Engine *engine) {
	engine->running = 0;
	world_init(&world);
	return renderer_init() == 1;
}

void engine_run(Engine *engine) {
	SDL_Event event;
	engine->running = 1;
	int is_holding_mouse = 0;
	int mx, my;

	while (engine->running) {
		Uint32 start_time = SDL_GetTicks();
		while (SDL_PollEvent(&event)) 
			switch (event.type) {
				case SDL_QUIT:
					engine->running = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						SDL_GetMouseState(&mx, &my);
						if (my < world.platform_height)
							is_holding_mouse = 1;
					} 

					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT && is_holding_mouse)
						is_holding_mouse = 0;
					break;
			}
		
		if (is_holding_mouse) {
			SDL_GetMouseState(&mx, &my);
			generate_sand(&world, mx, my);
		}

		Uint32 frame_time = SDL_GetTicks() - start_time;
		float dt = frame_time / 1000.0f;
		world_update(&world, dt);

		renderer_begin();
		renderer_draw(&world);
		renderer_end();	

		if (frame_time < TARGET_FPS)
			SDL_Delay(FRAME_TIME - frame_time); 
	}
}

void engine_shutdown(Engine *engine) {
	world_destroy(&world);
	renderer_shutdown();
}

static void generate_sand(World *world, int x, int y) {
	for (int i = x; i < x + SAND_SIZE && i < world->width; i++)
		for (int j = y; j < y + SAND_SIZE && j < world->height; j++)
			world->grid[j][i] = SAND;
	world->num_of_objects++;
}
