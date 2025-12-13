// world.h

#ifndef WORLD_H
#define WORLD_H


typedef struct {
	int num_of_objects;
	// more here later...
} World;

void world_init(World *world);
void world_update(World *world, float dt);
void world_destroy(World *world);

#endif 
