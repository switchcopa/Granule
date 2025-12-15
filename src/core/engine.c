#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "engine.h"
#include <stdlib.h>
#include <stdio.h> 
#include <time.h>
#include "../render/renderer.h"
#include "../world/world.h"

static void generate_entity(World *world, CellType entity_type, int x, int y);
static void assign_rand_color(World *world, CellType entity_type, int i, int j);

World world;

CellColor sand_colors[3] = {
    {228, 210, 150}, // light warm sand
    {220, 200, 135}, // slightly darker sand
    {235, 218, 165}  // soft yellow sand
};

CellColor wet_sand_colors[3] = {
    {185, 160, 95},  // darker brownish-yellow
    {170, 145, 85},  // muted wet sand
    {155, 130, 70}   // deep wet sand tone
};

CellColor water_color = {0, 105, 180};

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
			generate_entity(&world, engine->entity_summon_type,mx, my);
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

static void generate_entity(World *world, CellType entity_type, int x, int y) {
	int grid_x = x / BLOCK_SIZE;
	int grid_y = y / BLOCK_SIZE;

	for (int i = grid_x; i >= 0 && i < grid_x + world->generation_size && i < world->width; i++) {
		for (int j = grid_y; j >= 0 && j < grid_y + world->generation_size && j < world->height; j++) {
			if (world->grid[j][i] != EMPTY)
				continue;
			world->grid[j][i] = entity_type;
                        assign_rand_color(world, entity_type, i, j);
			world->num_of_objects++;
		}
	}
}

static void assign_rand_color(World *world, CellType entity_type, int i, int j) {
        int choice = rand() % 3;
        CellColor *colors;
        switch (entity_type) {
                case SAND:
                        colors = sand_colors;
                        break;
                case WET_SAND:
                        colors = wet_sand_colors;
                        break;
                case WATER:
                        world->color_buffer[j][i].r = 0;
                        world->color_buffer[j][i].g = 105;
                        world->color_buffer[j][i].b = 180;
			return;
                default:
                        break;
        }

        world->color_buffer[j][i].r = (uint8_t) colors[choice].r;
        world->color_buffer[j][i].g = (uint8_t) colors[choice].g;
        world->color_buffer[j][i].b = (uint8_t) colors[choice].b;
}
