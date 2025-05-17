#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

#include <cglm/cglm.h>

typedef struct
{
    vec3 position;
    versor orientation;
} Camera;

void renderer_init();
void renderer_draw(float delta_time);
void renderer_shutdown();

#endif