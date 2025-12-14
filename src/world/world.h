// world.h

#ifndef WORLD_H
#define WORLD_H

#define CELL_SIZE 4
#define MAX_SAND_STEPS 4

typedef enum {
	EMPTY,
	SAND
} CellType;

typedef struct {
	CellType **grid;
	float sand_speed;
	int width, height;
	int num_of_objects;
	int platform_height;
	// more here later...
} World;

int world_init(World *world);
void world_update(World *world, float dt);
void world_destroy(World *world);

#endif 
