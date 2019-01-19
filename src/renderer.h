#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

#include <cglm/cglm.h>

typedef struct
{
    vec3 position;
    versor orientation;
} Camera;

void rendererInit();
void rendererDraw(float deltaTime);
void rendererShutdown();

#endif