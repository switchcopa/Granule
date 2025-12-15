#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "engine.h"
#include <stdlib.h>
#include <stdio.h> 
#include "../../include/shared/colors.h"
#include <time.h>
#include "../render/renderer.h"
#include "../world/world.h"

static void generate_entities(World *world, CellType entity_type, int x, int y);
static void summon_entity(World *world, CellType entity_type, int i, int j);

World world;

int engine_init(Engine *engine) {
	engine->running = 0;
	engine->entity_summon_type = SAND;
        srand(time(NULL));

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
						if (my / BLOCK_SIZE < world.platform_height)
							is_holding_mouse = 1;
					} 

					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT && is_holding_mouse)
						is_holding_mouse = 0;
					break;
				case SDL_MOUSEWHEEL:
					if (event.wheel.y > 0 && world.generation_size < MAX_GENERATION_SIZE)
						world.generation_size += 1;
					else if (event.wheel.y < 0 && world.generation_size > 1)
						world.generation_size -= 1;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_e) {
						if (engine->entity_summon_type == SAND)
							engine->entity_summon_type = WATER;
						else  
							engine->entity_summon_type = SAND;

					}
					break;
			}
		
		if (is_holding_mouse) {
			SDL_GetMouseState(&mx, &my);
			generate_entities(&world, engine->entity_summon_type, mx, my);
		}

		world_update(&world, 1.0/TARGET_FPS);

		renderer_begin();
		renderer_draw(&world);
		SDL_GetMouseState(&mx, &my);
		renderer_draw_cursor(&world, mx, my);
		renderer_end();	
		
		Uint32 frame_time = SDL_GetTicks() - start_time;
		Uint32 frame_delay = 1000 / TARGET_FPS;
		if (frame_time < frame_delay)
			SDL_Delay(frame_delay - frame_time); 
	}
}

void engine_shutdown(void) {
	world_destroy(&world);
	renderer_shutdown();
}

static void generate_entities(World *world, CellType entity_type, int x, int y) {
	int grid_x = x / BLOCK_SIZE;
	int grid_y = y / BLOCK_SIZE;

	for (int i = grid_y; i < grid_y + world->generation_size && i < world->height; i++) {
		for (int j = grid_x; j < grid_x + world->generation_size && j < world->width; j++) {
			if (i >= 0 && j >= 0 && world->grid[i][j].type == EMPTY) {
				summon_entity(world, entity_type, i, j);
				world->num_of_objects++;
			}
		}
	}
}

static void summon_entity(World *world, CellType entity_type, int i, int j) {
	int choice = rand() % 3;
        world->grid[i][j].color = sand_colors[choice];
	world->grid[i][j].type = entity_type;
}
