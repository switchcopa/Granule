// world.h

#include <stdint.h>
#include "../../include/shared/colors.h"

#ifndef WORLD_H
#define WORLD_H

#define MAX_GENERATION_SIZE 20
#define BLOCKS_N 6
#define DEFAULT_TEMP 20.0f
#define MIN_TEMP_DELTA 0.01f
#define TEMP_DIFFUSION 0.15f
#define BOILING_TEMP 100.0f
#define EVAPORATE_PROBABILITY 20

typedef enum {
	NORMAL,
	BURNING,
	FREEZING,
	GAS
} CellState;

typedef enum {
	EMPTY,
	SAND,
	WATER,
	WET_SAND,
	STEAM,
	STONE
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
} World;

int world_init(World *world);
void world_update(World *world);
void world_destroy(World *world);

#endif 
