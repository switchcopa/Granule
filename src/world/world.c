#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "../render/renderer.h"
#include "world.h"

static void water_update(World *world, int i, int j);
static void sand_update(World *world, int i, int j);
static void sand_water_collide(World *world, int i, int j);
static void wetsand_update(World *world, int i, int j);
static void swap_cell_color(World *world, int i_1, int j_1, int i_2, int j_2);
static void change_cell_color(World *world, int i, int j, uint8_t r, uint8_t g, uint8_t b);
static void assign_rand_color(World *world, CellType entity_type, int i, int j);
static void swap(uint8_t *a, uint8_t *b);

static CellColor sand_colors[3] = {
    {228, 210, 150}, // light warm sand
    {220, 200, 135}, // slightly darker sand
    {235, 218, 165}  // soft yellow sand
};

static CellColor wet_sand_colors[3] = {
    {185, 160, 95},  // darker brownish-yellow
    {170, 145, 85},  // muted wet sand
    {155, 130, 70}   // deep wet sand tone
};


int world_init(World *world) {
	world->num_of_objects = 0;
	world->platform_height = (WINDOW_HEIGHT - 200) / BLOCK_SIZE;
	world->width = WINDOW_WIDTH / BLOCK_SIZE, world->height = world->platform_height;
	world->generation_size = 2;

	world->grid = malloc(sizeof(CellType *) * world->height);
	world->color_buffer = malloc(sizeof(CellColor *) * world->height);
        if (world->grid == NULL || world->color_buffer == NULL)
                return 0;

	for (int i = 0; i < world->height ; i++) {
		world->grid[i] = malloc(sizeof(CellType) * world->width);
		world->color_buffer[i] = malloc(sizeof(CellColor) * world->width);
                if (world->grid[i] == NULL || world->color_buffer[i] == NULL)
                        return 0;
		for (int j = 0; j < world->width; j++) {
			world->grid[i][j] = EMPTY;
			world->color_buffer[i][j].r = 255;
			world->color_buffer[i][j].g = 255;
			world->color_buffer[i][j].b = 255;
		}
	}
        
	// initialize more here later
        return 1;
}

void world_update(World *world, float dt) {
	for (int i = world->height - 1; i >= 0; i--)
		for (int j = 0; j < world->width; j++)
			switch (world->grid[i][j]) {
				case EMPTY:
					break;
				case SAND:
					sand_update(world, i, j);
					sand_water_collide(world, i, j);
					break;
				case WATER:
					water_update(world, i, j);
					break;
				case WET_SAND:
					wetsand_update(world, i, j);
					break;
				default:
					break;
			}
	// do more later	
}

void world_destroy(World *world) {
	for (int i = 0; i < world->height ; i++) {
                free(world->grid[i]);
                free(world->color_buffer[i]);
        }

        free(world->color_buffer);
        free(world->grid);
	// do more later
}

static void sand_update(World *world, int i, int j) {
	if (i < world->height - 1 && world->grid[i+1][j] == EMPTY) {
		world->grid[i][j] = EMPTY;
		world->grid[i+1][j] = SAND;
                swap_cell_color(world, i, j, i+1, j);
	} else if (i < world->height - 1 && j > 0 && j < world->width - 1 && world->grid[i+1][j] != EMPTY) {
		if (world->grid[i+1][j-1] == EMPTY) {
			world->grid[i][j] = EMPTY;
			world->grid[i+1][j-1] = SAND;
                        swap_cell_color(world, i, j, i+1, j-1);
		} else if (world->grid[i+1][j+1] == EMPTY) {
			world->grid[i][j] = EMPTY;
			world->grid[i+1][j+1] = SAND;
                        swap_cell_color(world, i, j, i+1, j+1);
		}
	}
}

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

static void change_cell_color(World *world, int i, int j, uint8_t r, uint8_t g, uint8_t b) {
        world->color_buffer[i][j].r = r;
        world->color_buffer[i][j].g = g;
        world->color_buffer[i][j].b = b;
}

static void swap_cell_color(World *world, int i_1, int j_1, int i_2, int j_2) {
        swap(&world->color_buffer[i_1][j_1].r, &world->color_buffer[i_2][j_2].r);
        swap(&world->color_buffer[i_1][j_1].g, &world->color_buffer[i_2][j_2].g);
        swap(&world->color_buffer[i_1][j_1].b, &world->color_buffer[i_2][j_2].b);
}

static void swap(uint8_t *a, uint8_t *b) {
        uint8_t c = *a;
        *a = *b;
        *b = c;
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
