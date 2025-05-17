#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

Mesh *mesh_load(const char *filename)
{
    Mesh *mesh = malloc(sizeof(Mesh));
    glm_vec3_zero(mesh->position);
    glm_quat_identity(mesh->orientation);

    const struct aiScene *scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);
    if (!scene) {
        free(mesh);
        return NULL;
    }

    assert(scene->mNumMeshes == 1);
    
    mesh->num_vertices = scene->mMeshes[0]->mNumVertices;
    mesh->vertices = malloc(sizeof(vec3) * mesh->num_vertices);
    
    mesh->num_normals = scene->mMeshes[0]->mNumVertices;
    mesh->normals = malloc(sizeof(vec3) * mesh->num_normals);
    
    mesh->num_faces = scene->mMeshes[0]->mNumFaces;
    mesh->faces = malloc(sizeof(Triangle) * mesh->num_faces);

    // Load vertices and normals
    for (uint32_t i = 0; i < mesh->num_vertices; i++)
    {
        mesh->vertices[i][0] = scene->mMeshes[0]->mVertices[i].x;
        mesh->vertices[i][1] = scene->mMeshes[0]->mVertices[i].y;
        mesh->vertices[i][2] = scene->mMeshes[0]->mVertices[i].z;

        mesh->normals[i][0] = scene->mMeshes[0]->mNormals[i].x;
        mesh->normals[i][1] = scene->mMeshes[0]->mNormals[i].y;
        mesh->normals[i][2] = scene->mMeshes[0]->mNormals[i].z;
        glm_vec3_normalize(mesh->normals[i]);
    }

    // Load faces and calculate face normals as average of vertex normals
    for (uint32_t i = 0; i < mesh->num_faces; i++)
    {
        assert(scene->mMeshes[0]->mFaces[i].mNumIndices == 3);
        
        mesh->faces[i].v1 = scene->mMeshes[0]->mFaces[i].mIndices[0];
        mesh->faces[i].v2 = scene->mMeshes[0]->mFaces[i].mIndices[1];
        mesh->faces[i].v3 = scene->mMeshes[0]->mFaces[i].mIndices[2];

        // Average the vertex normals for this face
        glm_vec3_copy(mesh->normals[mesh->faces[i].v1], mesh->faces[i].normal);
        glm_vec3_add(mesh->faces[i].normal, mesh->normals[mesh->faces[i].v2], mesh->faces[i].normal);
        glm_vec3_add(mesh->faces[i].normal, mesh->normals[mesh->faces[i].v3], mesh->faces[i].normal);
        glm_vec3_normalize(mesh->faces[i].normal);
    }

    aiReleaseImport(scene);
    return mesh;
}

void mesh_free(Mesh *mesh)
{
    if (mesh == NULL) return;
    
    if (mesh->vertices != NULL)
    {
        free(mesh->vertices);
        mesh->vertices = NULL;
    }
    
    if (mesh->normals != NULL)
    {
        free(mesh->normals);
        mesh->normals = NULL;
    }
    
    if (mesh->faces != NULL)
    {
        free(mesh->faces);
        mesh->faces = NULL;
    }
    
    free(mesh);
}