#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "config.h"
#include "renderer.h"
#include "window.h"
#include "mesh.h"
#include "math.h"

mat4 projectionMatrix;
mat4 viewMatrix;
vec3 forward;
Camera camera;
Mesh* mesh;

float rot = 0;

void rendererInit()
{
    camera.position[0] =  0;
    camera.position[1] =  0;
    camera.position[2] = -8;

    glm_quat_identity(camera.orientation);

    mesh = meshLoad("bin/assets/mesh/torus.obj");

    // projection matrix

    glm_mat4_identity(projectionMatrix);

    glm_perspective(
        glm_rad(60),
        (float)FRAMEBUFFER_WIDTH / (float)FRAMEBUFFER_HEIGHT,
        0.1f,
        100,
        projectionMatrix
    );
}

void rendererDrawMesh(Mesh* mesh, Camera* camera)
{
    // model matrix

    mat4 modelMatrix;
    glm_mat4_identity(modelMatrix);
    
    glm_translate(modelMatrix, mesh->position);
    glm_quat_rotate(modelMatrix, mesh->orientation, modelMatrix);

    // model view matrix

    mat4 modelViewMatrix;
    glm_mat4_mul(viewMatrix, modelMatrix, modelViewMatrix);
    
    // transformation matrix

    mat4 transformationMatrix;
    glm_mat4_mul(projectionMatrix, modelViewMatrix, transformationMatrix);

    // render faces
    
    for (int i = 0; i < mesh->numFaces; i++)
    {
        // backface culling

        vec3 normal;
        glm_vec3_copy(mesh->faces[i].normal, normal);
        glm_mat4_project(normal, modelMatrix, normal);

        if (glm_vec3_dot(forward, normal) <= 0) continue;

        // render face

        vec3 v1;
        glm_vec3_copy(mesh->vertices[mesh->faces[i].v1], v1);
        glm_mat4_project(v1, transformationMatrix, v1);
        
        vec3 v2;
        glm_vec3_copy(mesh->vertices[mesh->faces[i].v2], v2);
        glm_mat4_project(v2, transformationMatrix, v2);
        
        vec3 v3;
        glm_vec3_copy(mesh->vertices[mesh->faces[i].v3], v3);
        glm_mat4_project(v3, transformationMatrix, v3);

        windowDrawLine(v1[0], v1[1], v2[0], v2[1], 255, 255, 255);
        windowDrawLine(v2[0], v2[1], v3[0], v3[1], 255, 255, 255);
        windowDrawLine(v3[0], v3[1], v1[0], v1[1], 255, 255, 255);
    }
}

void rendererDraw(float deltaTime)
{
    // view matrix

    glm_mat4_identity(viewMatrix);
    glm_translate(viewMatrix, camera.position);
    glm_quat_rotate(viewMatrix, camera.orientation, viewMatrix);

    // calculate forward vector

    glm_quat_forward(camera.orientation, forward);

    // rotate mesh

    rot = rot + 0.03f * (deltaTime * (1.0f / FRAMERATE));
    if(rot > M_PI * 2)
        rot = 0;

    glm_quatv(mesh->orientation, rot, GLM_YUP);

    // render mesh

    rendererDrawMesh(mesh, &camera);
}

void rendererShutdown()
{
}
