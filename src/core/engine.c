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

#define BUF_SIZE 24

static void 	generate_entities(World *world, CellType entity_type, int x, int y);
static void 	summon_entity(World *world, CellType entity_type, int i, int j);
static void 	erase(World *world, int i, int j);
static void 	clear_cell(Cell *cell);
static void 	reset(World *world);
static void 	get_cursor(Engine *engine);
static void 	heat(World *world, int x, int y);

World world;

extern const char *block_names[];
const char *modes[MODES_N] = {"NONE", "ERASE", "HEAT"};

int engine_init(Engine *engine) {
	engine->running = 0;
	engine->palette = SAND;
	engine->mode = NONE;
        engine->hovered_cell = NULL;
        srand(time(NULL));

	world_init(&world);
	return renderer_init() == 1;
}

void engine_run(Engine *engine) {
	SDL_Event event;
	engine->running = 1;
	int is_holding_mouse = 0;
	
	while (engine->running) {
		Uint32 start_time = SDL_GetTicks();
		get_cursor(engine);
               	
		int cx = engine->cursor_x / BLOCK_SIZE, cy = engine->cursor_y / BLOCK_SIZE; 
                if (cx >= 0 && cx < world.width && cy >= 0 && cy < world.height)
                        engine->hovered_cell = &world.grid[cy][cx];
                else
                        engine->hovered_cell = NULL;

		while (SDL_PollEvent(&event)) 
			switch (event.type) {
				case SDL_QUIT:
					engine->running = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						if (engine->cursor_y / BLOCK_SIZE < world.platform_height)
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
				        if (event.key.keysym.sym == SDLK_e)
						engine->palette = (CellType)(engine->palette + 1) % BLOCKS_N;
					if (event.key.keysym.sym == SDLK_s)
                                                engine->mode = (Mode)(engine->mode + 1) % MODES_N;
					if (event.key.keysym.sym == SDLK_r)
						reset(&world);
					break;
			}

                         

		if (is_holding_mouse) {
			if (engine->mode == NONE)
				generate_entities(&world, engine->palette, engine->cursor_x, engine->cursor_y);
			else if (engine->mode == ERASE)
				erase(&world, engine->cursor_x, engine->cursor_y);
			else if (engine->mode == HEAT)
				heat(&world, engine->cursor_x, engine->cursor_y);	
		}

		world_update(&world);

		renderer_begin();
		renderer_draw(&world);

		char block_text[BUF_SIZE];
		snprintf(block_text, sizeof(block_text), "Palette: %s", block_names[engine->palette]);
		renderer_draw_text(block_text, 10, WINDOW_HEIGHT - 200);
		
		char mode_text[BUF_SIZE];
		snprintf(mode_text, sizeof(mode_text), "Mode: %s", modes[engine->mode]);
		renderer_draw_text(mode_text , WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT - 200);
                
                if (engine->hovered_cell) {
                        char temperature_text[BUF_SIZE];
			char type_text[BUF_SIZE];
                        char timer_text[BUF_SIZE];
                        snprintf(temperature_text, sizeof(temperature_text), "Temperature: %.1f", engine->hovered_cell->temperature);
                        snprintf(type_text, sizeof(type_text), "Cell Type: %s", block_names[engine->hovered_cell->type]);
                        snprintf(timer_text, sizeof(timer_text), "Ticks: %u", engine->hovered_cell->timer); 
                        
                        renderer_draw_text(temperature_text, WINDOW_WIDTH - 350, WINDOW_HEIGHT - 200);
                        renderer_draw_text(type_text       , WINDOW_WIDTH - 350, WINDOW_HEIGHT - 175);
                        renderer_draw_text(timer_text      , WINDOW_WIDTH - 350, WINDOW_HEIGHT - 150);
                }

		renderer_draw_cursor(&world, engine->cursor_x, engine->cursor_y);
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

	for (int i = grid_y; i < grid_y + world->generation_size && i < world->height; i++)
		for (int j = grid_x; j < grid_x + world->generation_size && j < world->width; j++)
			if (i >= 0 && j >= 0 && world->grid[i][j].type == EMPTY) {
				summon_entity(world, entity_type, i, j);
				world->num_of_objects++;
			}
}

static void summon_entity(World *world, CellType entity_type, int i, int j) {
	const CellColor *clr;
	switch (entity_type) {
		case SAND:
			clr = (CellColor *) sand_colors;
			break;
		case WATER:
                        world->grid[i][j].color = water_color;
	                world->grid[i][j].type = entity_type;
                        return;
		case WET_SAND:
			clr = (CellColor *) wet_sand_colors;
			break;
		case STEAM:
			world->grid[i][j].type = entity_type;
			world->grid[i][j].temperature = BOILING_TEMP;
			world->grid[i][j].state = GAS;
			return;
		case STONE:
			clr = (CellColor *) stone_colors;
			break;
                default:
                        return;
	}

	int choice = rand() % 3;
        world->grid[i][j].color = clr[choice];
	world->grid[i][j].type = entity_type;
}

static void erase(World *world, int x, int y) {
	int grid_x = x / BLOCK_SIZE;
	int grid_y = y / BLOCK_SIZE;

	for (int i = grid_y; i < grid_y + world->generation_size && i < world->height; i++)
		for (int j = grid_x; j < grid_x + world->generation_size && j < world->width; j++)
			if (i >= 0 && j >= 0 && world->grid[i][j].type != EMPTY)
				clear_cell(&world->grid[i][j]);
}

static void reset(World *world) {
	for (int i = 0; i < world->height; i++)
		for (int j = 0; j < world->width; j++)
			clear_cell(&world->grid[i][j]);
}

static void clear_cell(Cell *cell) {
	cell->type = EMPTY;
	cell->color = (CellColor) {0, 0, 0};
	cell->state = NORMAL;
	cell->temperature = DEFAULT_TEMP;
	cell->timer = 0U;
}

static void get_cursor(Engine *engine) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	engine->cursor_x = mx, engine->cursor_y = my;
}

static void heat(World *world, int x, int y) {
	int grid_x = x / BLOCK_SIZE;	
	int grid_y = y / BLOCK_SIZE;

	for (int i = grid_y; i < grid_y + world->generation_size && i < world->height; i++)
		for (int j = grid_x; j < grid_x + world->generation_size && j < world->width; j++)
			if (i >= 0 && j >= 0 && world->grid[i][j].type != EMPTY)
				world->grid[i][j].temperature += (float) world->generation_size / HEATING_SPEED;
}
