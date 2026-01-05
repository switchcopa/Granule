#include "core/engine.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	Engine engine;
	if (!engine_init(&engine)) {
		fprintf(stderr, "failed to initialize engine\n");
		return EXIT_FAILURE;
	}

	engine_run(&engine);	
	engine_shutdown();
	
	return EXIT_SUCCESS;
}
