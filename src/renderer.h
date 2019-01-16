#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

typedef struct
{
    vec3 position, target;
} camera_t;

typedef struct
{
    int v1, v2, v3;
} triangle_t;

typedef struct
{
    vec3* vertices;
    int num_vertices;
    triangle_t* triangles;
    int num_triangles;
    float rotation;
} mesh_t;

void renderer_init();
void renderer_draw(float dt);
void renderer_shutdown();

#endif