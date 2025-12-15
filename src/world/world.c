#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "../render/renderer.h"
#include "../../include/shared/colors.h"
#include "world.h"

static void water_update(World *world, int i, int j);
static void sand_update(World *world, int i, int j);
static void sand_water_collide(World *world, int i, int j);
static void wetsand_update(World *world, int i, int j);
static void swap_cells(World *world, int i_1, int j_1, int i_2, int j_2);

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
		
		for (int j = 0; j < world->width; j++) {
			world->grid[i][j].type = EMPTY;
			world->grid[i][j].color = (CellColor) {0, 0, 0};
		}
	}

        return 1;
}

void world_update(World *world, float dt) {
	for (int i = world->height - 1; i >= 0; i--)
		for (int j = 0; j < world->width; j++)
			switch (world->grid[i][j].type) {
				case EMPTY:
					break;
				case SAND:
					sand_update(world, i, j);
					//sand_water_collide(world, i, j);
					break;
				case WATER:
					//water_update(world, i, j);
					break;
				case WET_SAND:
					//wetsand_update(world, i, j);
					break;
				default:
					break;
			}
	// do more later	
}

void world_destroy(World *world) {
	for (int i = 0; i < world->height ; i++)
                free(world->grid[i]);

        free(world->grid);
}

static void sand_update(World *world, int i, int j) {
	if (i < world->height - 1 && world->grid[i+1][j].type == EMPTY) {
		swap_cells(world, i, j, i+1, j);	
	} else if (i < world->height - 1 && j > 0 && j < world->width - 1 && world->grid[i+1][j].type != EMPTY) {
		if (world->grid[i+1][j-1].type == EMPTY) {
			swap_cells(world, i, j, i+1, j-1);	
		} else if (world->grid[i+1][j+1].type == EMPTY) {
			swap_cells(world, i, j, i+1, j+1);
		}
	}
}

/*
static void water_update(World *world, int i, int j) {
	int decision = rand() % 2;
	if (i < world->height - 1 && j < world->width - 1) {
		if (world->grid[i+1][j] == EMPTY) {
			world->grid[i][j] = EMPTY;
			world->grid[i+1][j] = WATER;
                        swap_cell_color(world, i, j, i+1, j);
		} else if (world->grid[i+1][j] == SAND) {
			world->grid[i][j] = EMPTY;
			world->grid[i+1][j] = WET_SAND;
			change_cell_color(world, i, j, 255, 255, 255);
			assign_rand_color(world, WET_SAND, i + 1, j);
		} else {

			if (j > 0 && world->grid[i][j-1] == EMPTY && decision == 0) {
				world->grid[i][j] = EMPTY;
				world->grid[i][j-1] = WATER;
                                swap_cell_color(world, i, j, i, j-1);
			}
			else if (world->grid[i][j+1] == EMPTY && decision == 1) {
				world->grid[i][j] = EMPTY;
				world->grid[i][j+1] = WATER;
                                swap_cell_color(world, i, j, i, j+1);
			}
		}
	}
}

static void sand_water_collide(World *world, int i, int j) {
	if (i < world->height - 1 && j < world->width - 1) {
		if (j > 0 && world->grid[i+1][j-1] == WATER) {
			world->grid[i][j] = EMPTY;
			world->grid[i+1][j-1] = WET_SAND;
			change_cell_color(world, i, j, 255, 255, 255);
			assign_rand_color(world, WET_SAND, i + 1, j-1);
		} else if (world->grid[i+1][j] == WATER) {
			world->grid[i][j] = EMPTY;
			world->grid[i+1][j] = WET_SAND;
			change_cell_color(world, i, j, 255, 255, 255);
			assign_rand_color(world, WET_SAND, i + 1, j);
		} else if (world->grid[i+1][j+1] == WATER) {
			world->grid[i][j] = EMPTY;
			world->grid[i+1][j+1] = WET_SAND;
			change_cell_color(world, i, j, 255, 255, 255);
			assign_rand_color(world, WET_SAND, i + 1, j+1);
		}
	}
}

static void wetsand_update(World *world, int i, int j) {
	int decision = rand() % 4;

	if (i < world->height - 1 && j < world->width - 1) {
		switch (world->grid[i+1][j]) {
			case EMPTY:
				world->grid[i][j] = EMPTY;
				world->grid[i+1][j] = WET_SAND;
				break;
			case WATER:
				if (decision != 1) break;
				world->grid[i][j] = WATER;
				world->grid[i+1][j] = WET_SAND;
				break;
			default:
				break;
		}
	}
}
*/

static void swap_cells(World *world, int i_1, int j_1, int i_2, int j_2) {
	CellType temp1 = world->grid[i_1][j_1].type;
	world->grid[i_1][j_1].type = world->grid[i_2][j_2].type;
	world->grid[i_2][j_2].type = temp1;

	CellColor temp2 = world->grid[i_1][j_1].color;
	world->grid[i_1][j_1].color = world->grid[i_2][j_2].color;
	world->grid[i_2][j_2].color = temp2;
}
