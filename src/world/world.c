#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h> 
#include "../render/renderer.h"
#include "../../include/shared/colors.h"
#include "world.h"
#include <stdint.h>

static void sand_update(World *world, int i, int j);

static void water_update(World *world, int i, int j);

static void wetsand_update(World *world, int i, int j);

static void sand_water_collide(World *world, int i_1, int j_1, int i_2, int j_2);

static void steam_update(World *world, int i, int j);

static void evaporate(World *world, int i, int j);

static void swap_cells(Cell *cell1, Cell *cell2);

static void init_cell(Cell *cell);

static void inc_timer(Cell *cell);

static void apply_temperature(World *world, int i, int j);

const char *block_names[BLOCKS_N] = {"NONE", "SAND", "WATER", "WET_SAND", "STEAM", "STONE"};

int world_init(World *world) {
	world->num_of_objects = 0;
	world->platform_height = (WINDOW_HEIGHT - 200) / BLOCK_SIZE;
	world->width = WINDOW_WIDTH / BLOCK_SIZE, world->height = world->platform_height;
	world->generation_size = 2;

	world->grid = malloc(sizeof(Cell *) * world->height);
        if (world->grid == NULL)
                return 0;
	for (int i = 0; i < world->height; i++) {
		world->grid[i] = malloc(sizeof(Cell) * world->width);
		if (world->grid[i] == NULL) {
			for (int k = 0; i < k; k++)
				free(world->grid[i]);
			free(world->grid);
			return 0;
		}
		
		for (int j = 0; j < world->width; j++)
			init_cell(&world->grid[i][j]);
	}

        return 1;
}

void world_update(World *world) {
	for (int i = world->height - 1; i >= 0; i--)
		for (int j = 0; j < world->width; j++) {
			apply_temperature(world, i, j);
			inc_timer(&world->grid[i][j]);
			switch (world->grid[i][j].type) {
				case SAND:
					sand_update(world, i, j);
					break;
				case WATER:
					water_update(world, i, j);
					break;
				case WET_SAND:
					wetsand_update(world, i, j);
					break;
				case STEAM:
					steam_update(world, i, j);
					break;
				default:
					break;
			}
		}
}

void world_destroy(World *world) {
	for (int i = 0; i < world->height ; i++)
                free(world->grid[i]);

        free(world->grid);
}

static void sand_update(World *world, int i, int j) {
	if (i < world->height - 1 && world->grid[i+1][j].type == EMPTY) {
		swap_cells(&world->grid[i][j], &world->grid[i+1][j]);
	} else if (i < world->height - 1 && world->grid[i+1][j].type != EMPTY) {
		if (j > 0 && world->grid[i+1][j-1].type == EMPTY)
			swap_cells(&world->grid[i][j], &world->grid[i+1][j-1]);
		else if (j < world->width - 1 && world->grid[i+1][j+1].type == EMPTY)
			swap_cells(&world->grid[i][j], &world->grid[i+1][j+1]);
		else if (i < world->height - 1  && world->grid[i+1][j].type == WATER)
			sand_water_collide(world, i, j, i+1, j);
	}
}

static void water_update(World *world, int i, int j) {
	if (world->grid[i][j].temperature >= BOILING_TEMP && i > 0 && world->grid[i-1][j].type == EMPTY) {
		int evaporate_p = rand() % EVAPORATE_PROBABILITY;
		if (evaporate_p == 1) {
			evaporate(world, i, j);
			return;
		}
	}

	if (i < world->height - 1 && world->grid[i+1][j].type == EMPTY) {
		swap_cells(&world->grid[i][j], &world->grid[i+1][j]);
		return;
	}

	int decision = rand() % 2;
	if (j > 0 && world->grid[i][j-1].type == EMPTY && decision == 0)
		swap_cells(&world->grid[i][j], &world->grid[i][j-1]);
	else if (j < world->width - 1 && world->grid[i][j+1].type == EMPTY && decision == 1)
		swap_cells(&world->grid[i][j], &world->grid[i][j+1]);

	
	if (i < world->height - 1 && world->grid[i+1][j].type == SAND)
		sand_water_collide(world, i, j, i+1, j);			
	else if (j < world->width - 1 && world->grid[i][j+1].type == SAND)
		sand_water_collide(world, i, j, i, j+1);			
	else if (j > 0 && world->grid[i][j-1].type == SAND)
		sand_water_collide(world, i, j, i, j-1);			
	else if (i < world->height - 1 && j < world->width - 1 && world->grid[i+1][j+1].type == SAND)
		sand_water_collide(world, i, j, i+1, j+1);
	else if (i < world->height - 1 && j > 0 && world->grid[i+1][j-1].type == SAND)
		sand_water_collide(world, i, j, i+1, j-1);
}

static void steam_update(World *world, int i, int j) {
        int nrand = rand() % 10;
        if (i > 0 && world->grid[i - 1][j].type == EMPTY && nrand == 0) {
                swap_cells(&world->grid[i][j], &world->grid[i - 1][j]);
                return;
        }

        int dir = (rand() & 1) ? -1 : 1;

        if (i > 0 && j + dir >= 0 && j + dir < world->width &&
                world->grid[i - 1][j + dir].type == EMPTY) {
                swap_cells(&world->grid[i][j], &world->grid[i - 1][j + dir]);
                return;
        }

        if (j + dir >= 0 && j + dir < world->width &&
                world->grid[i][j + dir].type == EMPTY) {
                swap_cells(&world->grid[i][j], &world->grid[i][j + dir]);
        }
}

static void sand_water_collide(World *world, int i_1, int j_1, int i_2, int j_2) {
	int choice = rand() % 3;
	world->grid[i_1][j_1].type = EMPTY;
	world->grid[i_1][j_1].color = (CellColor) {0, 0, 0};
	world->grid[i_2][j_2].type = WET_SAND;
	world->grid[i_2][j_2].color = wet_sand_colors[choice];
	world->grid[i_2][j_2].timer = world->grid[i_1][j_1].timer;
	world->grid[i_1][j_1].timer = 0U;
	world->grid[i_2][j_2].temperature = world->grid[i_1][j_1].temperature;
}

static void wetsand_update(World *world, int i, int j) {
	int decision = rand() % 7;
	int fall_p = rand() % 3000;

	Cell *cell = &world->grid[i][j];
	if (i < world->height - 1) {
		Cell *below = &world->grid[i+1][j];
		switch (below->type) {
			case EMPTY:
				swap_cells(cell, below);
				break;
			case WATER:
				if (decision != 1) break;
				swap_cells(cell, below);
				break;
			case SAND:
				if (fall_p == 0)
					swap_cells(cell, below);
				break;
			default:
				break;
		}
	}
}

static void evaporate(World *world, int i, int j) {
	Cell *cell = &world->grid[i][j];
	cell->type = STEAM;
	cell->timer = 0U;
	cell->state = GAS;
}

static void swap_cells(Cell *cell_1, Cell *cell_2) {
	CellType type = cell_1->type;
	cell_1->type = cell_2->type;
	cell_2->type = type;

	CellColor color = cell_1->color;
	cell_1->color = cell_2->color;
	cell_2->color = color;

	float temp = cell_1->temperature;
	cell_1->temperature = cell_2->temperature;
	cell_2->temperature = temp;

	uint16_t timer = cell_1->timer;
	cell_1->timer = cell_2->timer;
	cell_2->timer = timer;

	CellState state = cell_1->state;
	cell_1->state = cell_2->state;
	cell_2->state = state;
}

static void init_cell(Cell *cell) {
	cell->type = EMPTY;
	cell->color = (CellColor) {0, 0, 0};
	cell->temperature = DEFAULT_TEMP; 
	cell->timer = 0U;
	cell->state = NORMAL;
}

static void inc_timer(Cell *cell) {
	cell->timer++;
}

static void apply_temperature(World *world, int i, int j) {
        Cell *c = &world->grid[i][j];
        if (c->type == EMPTY)
                return;

        float t = c->temperature;
        float delta_sum = 0.0f;

        for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0)
                                continue;

                        int ni = i + di;
                        int nj = j + dj;

                        if (ni < 0 || ni >= world->height ||
                                nj < 0 || nj >= world->width)
                                continue;

                        Cell *n = &world->grid[ni][nj];
                        if (n->type == EMPTY)
                                continue;

                            
                        float diff = t - n->temperature;
                        if (fabsf(diff) < MIN_TEMP_DELTA)
                                continue;

                        float transfer = diff * TEMP_DIFFUSION;
                        n->temperature += transfer;
                        delta_sum -= transfer;
                }
            }

        c->temperature += delta_sum;
}
