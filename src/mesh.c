#include <assert.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

Mesh* meshLoad(const char *filename)
{
    Mesh* mesh = malloc(sizeof(Mesh));

    glm_vec3_zero(mesh->position);
    glm_quat_identity(mesh->orientation);

    const struct aiScene* scene = NULL;

    scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);

    assert(scene->mNumMeshes == 1);

    mesh->numVertices = scene->mMeshes[0]->mNumVertices;
    mesh->vertices = malloc(sizeof(vec3) * mesh->numVertices);

    mesh->numNormals = scene->mMeshes[0]->mNumVertices;
    mesh->normals = malloc(sizeof(vec3) * mesh->numNormals);

    mesh->numFaces = scene->mMeshes[0]->mNumFaces;
    mesh->faces = malloc(sizeof(Triangle) * mesh->numFaces);

    for (int i = 0; i < mesh->numVertices; i++)
    {
        mesh->vertices[i][0] = scene->mMeshes[0]->mVertices[i].x;
        mesh->vertices[i][1] = scene->mMeshes[0]->mVertices[i].y;
        mesh->vertices[i][2] = scene->mMeshes[0]->mVertices[i].z;

        mesh->normals[i][0] = scene->mMeshes[0]->mNormals[i].x;
        mesh->normals[i][1] = scene->mMeshes[0]->mNormals[i].y;
        mesh->normals[i][2] = scene->mMeshes[0]->mNormals[i].z;
    }

    for (int i = 0; i < mesh->numFaces; i++)
    {
        assert(scene->mMeshes[0]->mFaces[i].mNumIndices == 3);

        mesh->faces[i].v1 = scene->mMeshes[0]->mFaces[i].mIndices[0];
        mesh->faces[i].v2 = scene->mMeshes[0]->mFaces[i].mIndices[1];
        mesh->faces[i].v3 = scene->mMeshes[0]->mFaces[i].mIndices[2];

        vec3 e1;
        glm_vec3_sub(
            mesh->vertices[mesh->faces[i].v2],
            mesh->vertices[mesh->faces[i].v1],
            e1
        );

        vec3 e2;
        glm_vec3_sub(
            mesh->vertices[mesh->faces[i].v3],
            mesh->vertices[mesh->faces[i].v1],
            e2
        );

        glm_vec3_cross(e1, e2, mesh->faces[i].normal);
    }

    return mesh;
}