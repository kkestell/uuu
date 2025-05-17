#ifndef MESH_H
#define MESH_H

#include <stdint.h>

#include <cglm/cglm.h>

typedef struct
{
    uint32_t v1, v2, v3;
    vec3 normal;
} Triangle;

typedef struct
{
    vec3 position;
    versor orientation;

    vec3 *vertices;
    uint32_t num_vertices;

    vec3 *normals;
    uint32_t num_normals;

    vec3 *uvs;
    uint32_t num_uvs;

    Triangle *faces;
    uint32_t num_faces;
} Mesh;

Mesh *mesh_load(const char *filename);
void mesh_free(Mesh *mesh);

#endif