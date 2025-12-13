#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "../render/renderer.h"
#include "world.h"

int world_init(World *world) {
	world->num_of_objects = 0;
	world->platform_height = WINDOW_HEIGHT - 200;
	world->width = WINDOW_WIDTH, world->height = world->platform_height;
	world->grid = malloc(sizeof(CellType *) * world->height);
        if (world->grid == NULL)
                return 0;

	for (int i = 0; i < world->height ; i++) {
		world->grid[i] = malloc(sizeof(CellType) * WINDOW_WIDTH);
                if (world->grid[i] == NULL)
                        return 0;
		for (int j = 0; j < WINDOW_WIDTH; j++)
			world->grid[i][j] = EMPTY;
	}
        
        return 1;
	// initialize more here later
}

void world_update(World *world, float dt) {
	for (int i = 0; i < world->height; i++)
		for (int j = 0; j < world->width; j++)
			switch (world->grid[i][j]) {
				case EMPTY:
					break;
				case SAND:
					if (i < world->height - 1 && world->grid[i+1][j] == EMPTY) {
						world->grid[i][j] = EMPTY;
						world->grid[i+1][j] = SAND;
					} else if (i < world->height - 1 && j > 0 && j < world->width - 1 && world->grid[i+1][j] != EMPTY) {
						if (world->grid[i+1][j-1] == EMPTY) {
							world->grid[i][j] = EMPTY;
							world->grid[i+1][j-1] = SAND;
						} else if (world->grid[i+1][j+1] == EMPTY) {
							world->grid[i][j] = EMPTY;
							world->grid[i+1][j+1] = SAND;
						}
					}
			}

	// do more later	
}

void world_destroy(World *world) {
	for (int i = 0; i < world->height ; i++)
                free(world->grid[i]);

        free(world->grid);
	// do more later
}
