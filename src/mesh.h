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

    vec3* vertices;
    uint32_t numVertices;

    vec3* normals;
    uint32_t numNormals;

    vec3* uvs;
    uint32_t numUvs;

    Triangle* faces;
    uint32_t numFaces;
} Mesh;

Mesh* meshLoad(const char *filename);

#endif