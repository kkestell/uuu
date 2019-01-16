#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "config.h"
#include "renderer.h"
#include "window.h"

mat4 projection_matrix;
camera_t camera;
mesh_t mesh;

void renderer_init()
{
    camera.position[0] = 0;
    camera.position[1] = 0;
    camera.position[2] = 8;

    camera.target[0] = 0;
    camera.target[1] = 0;
    camera.target[2] = 0;

    mesh.num_vertices = 4;
    mesh.vertices = (vec3*)malloc(sizeof(vec3) * mesh.num_vertices);

    mesh.num_triangles = 2;
    mesh.triangles = (triangle_t*)malloc(
        sizeof(triangle_t) * mesh.num_triangles
    );

    // bl
    mesh.vertices[0][0] = -1;
    mesh.vertices[0][1] = -1;
    mesh.vertices[0][2] =  0;

    // tl
    mesh.vertices[1][0] = -1;
    mesh.vertices[1][1] =  1;
    mesh.vertices[1][2] =  0;
    
    // tr
    mesh.vertices[2][0] =  1;
    mesh.vertices[2][1] =  1;
    mesh.vertices[2][2] =  0;
    
    // br
    mesh.vertices[3][0] =  1;
    mesh.vertices[3][1] = -1;
    mesh.vertices[3][2] =  0;

    // bl, tl, tr
    mesh.triangles[0].v1 = 0;
    mesh.triangles[0].v2 = 1;
    mesh.triangles[0].v3 = 2;

    // tr, br, bl
    mesh.triangles[1].v1 = 2;
    mesh.triangles[1].v2 = 3;
    mesh.triangles[1].v3 = 0;

    // projection matrix

    glm_mat4_identity(projection_matrix);

    glm_perspective(
        glm_rad(60),
        (float)FRAMEBUFFER_WIDTH / (float)FRAMEBUFFER_HEIGHT,
        0.1f,
        100,
        projection_matrix
    );
}

void project(vec3 v, mat4 m)
{
    vec4 p;
    glm_vec4(v, 1, p);

    glm_mat4_mulv(m, p, p);
    
    glm_vec4_divs(p, p[3], p);

    p[0] =  p[0] * FRAMEBUFFER_WIDTH  + FRAMEBUFFER_WIDTH  / 2;
    p[1] = -p[1] * FRAMEBUFFER_HEIGHT + FRAMEBUFFER_HEIGHT / 2;

    glm_vec3(p, v);
}

void renderer_draw_mesh(mesh_t* mesh, camera_t* camera)
{
    // model matrix

    /*
    mesh->rotation = mesh->rotation + 0.1f * (dt * (1.0f / FRAMERATE));
    if(mesh->rotation > M_PI * 2)
        mesh->rotation = 0;
    */

    mat4 model_matrix;
    glm_mat4_identity(model_matrix);

    // view matrix

    mat4 view_matrix;
    glm_mat4_identity(view_matrix);
    glm_lookat(camera->position, camera->target, GLM_YUP, view_matrix);

    // transformation matrix

    mat4 transformation_matrix;
    glm_mat4_identity(transformation_matrix);
    glm_mat4_mulN(
        (mat4 *[]) {
            &projection_matrix,
            &view_matrix,
            &model_matrix
        },
        3,
        transformation_matrix
    );

    // render
    
    for (int i = 0; i < mesh->num_triangles; i++)
    {
        vec3 v1;
        glm_vec3_copy(mesh->vertices[mesh->triangles[i].v1], v1);
        project(v1, transformation_matrix);
        
        vec3 v2;
        glm_vec3_copy(mesh->vertices[mesh->triangles[i].v2], v2);
        project(v2, transformation_matrix);
        
        vec3 v3;
        glm_vec3_copy(mesh->vertices[mesh->triangles[i].v3], v3);
        project(v3, transformation_matrix);

        window_draw_line(v1[0], v1[1], v2[0], v2[1], 255, 255, 255);
        window_draw_line(v2[0], v2[1], v3[0], v3[1], 255, 255, 255);
        window_draw_line(v3[0], v3[1], v1[0], v1[1], 255, 255, 255);

        window_put_pixel(v1[0], v1[1], 255, 0, 0);
        window_put_pixel(v2[0], v2[1], 255, 0, 0);
        window_put_pixel(v3[0], v3[1], 255, 0, 0);
    }
}

void renderer_draw(float dt)
{
    renderer_draw_mesh(&mesh, &camera);
}

void renderer_shutdown()
{
    free(mesh.vertices);
}
