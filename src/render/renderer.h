// renderer.h

#ifndef RENDERER_H
#define RENDERER_H


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS)


int renderer_init(void);
void renderer_begin(void);
void renderer_draw(void);
void renderer_end(void);
void renderer_shutdown(void);

#endif
