// world.h

#include <stdint.h>
#include "../../include/shared/colors.h"

#ifndef WORLD_H
#define WORLD_H

#define CELL_SIZE 4
#define MAX_GENERATION_SIZE 20

typedef enum {
	NORMAL,
	BURNING,
	FREEZING
} CellState;

typedef enum {
	EMPTY,
	SAND,
	WATER,
	WET_SAND
} CellType;

typedef struct {
	CellType type;
	CellColor color;
	CellState state;
	float temperature;
	uint16_t timer;
} Cell;

typedef struct {
	Cell **grid;
	int width, height;
	int num_of_objects;
	int platform_height;
	uint8_t generation_size;
	// more here later...
} World;

int world_init(World *world);
void world_update(World *world, float dt);
void world_destroy(World *world);

#endif 
