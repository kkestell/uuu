#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

typedef struct
{
    vec3 position, target;
} Camera;

typedef struct
{
    int v1, v2, v3;
} Triangle;

typedef struct
{
    vec3* vertices;
    int num_vertices;
    Triangle* triangles;
    int num_triangles;
    float rotation;
} Mesh;

void rendererInit();
void rendererDraw(float dt);
void rendererShutdown();

#endif