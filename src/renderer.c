#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <cglm/cglm.h>

#include "config.h"
#include "renderer.h"
#include "window.h"
#include "mesh.h"
#include "math.h"

mat4 projection_matrix;
mat4 view_matrix;
vec3 forward;
Camera camera;
Mesh *mesh;
float *z_buffer;

float rot = 0;

void renderer_init(const char *mesh_filename)
{
    camera.position[0] = 0.0f;
    camera.position[1] = 1.5f;
    camera.position[2] = -6.0f;

    glm_quat_identity(camera.orientation);

    mesh = mesh_load(mesh_filename);

    // projection matrix
    glm_mat4_identity(projection_matrix);
    glm_perspective(
        glm_rad(60),
        (float)FRAMEBUFFER_WIDTH / (float)FRAMEBUFFER_HEIGHT,
        0.1f,
        100,
        projection_matrix);

    // allocate z-buffer
    z_buffer = malloc(FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * sizeof(float));
}

void renderer_draw_filled_triangle(
    int32_t x1, int32_t y1, float z1,
    int32_t x2, int32_t y2, float z2,
    int32_t x3, int32_t y3, float z3,
    float brightness)
{
    uint8_t red = (uint8_t)(brightness * 255.0f);
    uint8_t green = (uint8_t)(brightness * 255.0f);
    uint8_t blue = (uint8_t)(brightness * 255.0f);

    // sort vertices by y
    int32_t temp_int;
    float temp_float;
    if (y2 < y1)
    {
        temp_int = x1;
        x1 = x2;
        x2 = temp_int;
        temp_int = y1;
        y1 = y2;
        y2 = temp_int;
        temp_float = z1;
        z1 = z2;
        z2 = temp_float;
    }
    if (y3 < y1)
    {
        temp_int = x1;
        x1 = x3;
        x3 = temp_int;
        temp_int = y1;
        y1 = y3;
        y3 = temp_int;
        temp_float = z1;
        z1 = z3;
        z3 = temp_float;
    }
    if (y3 < y2)
    {
        temp_int = x2;
        x2 = x3;
        x3 = temp_int;
        temp_int = y2;
        y2 = y3;
        y3 = temp_int;
        temp_float = z2;
        z2 = z3;
        z3 = temp_float;
    }

    int32_t dy1 = y2 - y1;
    int32_t dx1 = x2 - x1;
    float dz1 = z2 - z1;

    int32_t dy2 = y3 - y1;
    int32_t dx2 = x3 - x1;
    float dz2 = z3 - z1;

    int32_t dy3 = y3 - y2;
    int32_t dx3 = x3 - x2;
    float dz3 = z3 - z2;

    float dax_step = 0.0f, dbx_step = 0.0f;
    float daz_step = 0.0f, dbz_step = 0.0f;
    float dx3_step = 0.0f, dz3_step = 0.0f;

    if (dy1)
    {
        dax_step = (float)dx1 / (float)abs(dy1);
        daz_step = dz1 / (float)abs(dy1);
    }
    if (dy2)
    {
        dbx_step = (float)dx2 / (float)abs(dy2);
        dbz_step = dz2 / (float)abs(dy2);
    }
    if (dy3)
    {
        dx3_step = (float)dx3 / (float)abs(dy3);
        dz3_step = dz3 / (float)abs(dy3);
    }

    // top part
    if (dy1)
    {
        for (int32_t row = y1; row < y2; row++)
        {
            float alpha = (float)(row - y1);
            int32_t ax = (int32_t)((float)x1 + alpha * dax_step);
            int32_t bx = (int32_t)((float)x1 + alpha * dbx_step);

            float az = z1 + alpha * daz_step;
            float bz = z1 + alpha * dbz_step;

            if (ax > bx)
            {
                temp_int = ax;
                ax = bx;
                bx = temp_int;
                temp_float = az;
                az = bz;
                bz = temp_float;
            }

            float z_span = (bx == ax) ? 0.0f : (bz - az) / (float)(bx - ax);
            float z_val = az;

            for (int32_t col = ax; col < bx; col++)
            {
                if (col >= 0 && col < FRAMEBUFFER_WIDTH &&
                    row >= 0 && row < FRAMEBUFFER_HEIGHT)
                {
                    uint32_t idx = row * FRAMEBUFFER_WIDTH + col;
                    if (z_val < z_buffer[idx])
                    {
                        z_buffer[idx] = z_val;
                        window_put_pixel(col, row, red, green, blue);
                    }
                }
                z_val += z_span;
            }
        }
    }

    // bottom part
    if (dy3)
    {
        for (int32_t row = y2; row < y3; row++)
        {
            float alpha = (float)(row - y2);
            int32_t ax = (int32_t)((float)x2 + alpha * dx3_step);
            int32_t bx = (int32_t)((float)x1 + (float)(row - y1) * dbx_step);

            float az = z2 + alpha * dz3_step;
            float bz = z1 + (float)(row - y1) * dbz_step;

            if (ax > bx)
            {
                temp_int = ax;
                ax = bx;
                bx = temp_int;
                temp_float = az;
                az = bz;
                bz = temp_float;
            }

            float z_span = (bx == ax) ? 0.0f : (bz - az) / (float)(bx - ax);
            float z_val = az;

            for (int32_t col = ax; col < bx; col++)
            {
                if (col >= 0 && col < FRAMEBUFFER_WIDTH &&
                    row >= 0 && row < FRAMEBUFFER_HEIGHT)
                {
                    uint32_t idx = row * FRAMEBUFFER_WIDTH + col;
                    if (z_val < z_buffer[idx])
                    {
                        z_buffer[idx] = z_val;
                        window_put_pixel(col, row, red, green, blue);
                    }
                }
                z_val += z_span;
            }
        }
    }
}

void renderer_draw_mesh(Mesh *mesh)
{
    // model matrix
    mat4 model_matrix;
    glm_mat4_identity(model_matrix);

    glm_translate(model_matrix, mesh->position);
    glm_quat_rotate(model_matrix, mesh->orientation, model_matrix);

    // model view matrix
    mat4 model_view_matrix;
    glm_mat4_mul(view_matrix, model_matrix, model_view_matrix);

    // transformation matrix
    mat4 transformation_matrix;
    glm_mat4_mul(projection_matrix, model_view_matrix, transformation_matrix);

    // light direction in view space
    vec3 light_dir = {-1.0f, 0.1f, 1.0f};
    glm_vec3_normalize(light_dir);

    // render faces
    for (uint32_t i = 0; i < mesh->num_faces; i++)
    {
        // get face normal and transform to view space
        vec3 normal;
        glm_vec3_copy(mesh->faces[i].normal, normal);
        vec3 view_normal;
        glm_mat4_mulv3(model_view_matrix, normal, 0, view_normal);
        glm_vec3_normalize(view_normal);

        // backface culling in view space
        if (view_normal[2] < 0)
        {
            continue;
        }

        // calculate lighting using view space vectors
        float brightness = glm_vec3_dot(view_normal, light_dir);
        brightness = glm_clamp(brightness * 0.8f + 0.2f, 0.0f, 1.0f);

        // project vertices
        vec3 v1, v2, v3;
        vec3 projected1, projected2, projected3;

        glm_vec3_copy(mesh->vertices[mesh->faces[i].v1], v1);
        glm_vec3_copy(mesh->vertices[mesh->faces[i].v2], v2);
        glm_vec3_copy(mesh->vertices[mesh->faces[i].v3], v3);

        glm_project(v1, transformation_matrix, (vec4){0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT}, projected1);
        glm_project(v2, transformation_matrix, (vec4){0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT}, projected2);
        glm_project(v3, transformation_matrix, (vec4){0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT}, projected3);

        // draw triangle with lighting
        renderer_draw_filled_triangle(
            (int32_t)projected1[0], (int32_t)projected1[1], projected1[2],
            (int32_t)projected2[0], (int32_t)projected2[1], projected2[2],
            (int32_t)projected3[0], (int32_t)projected3[1], projected3[2],
            brightness);
    }
}

void renderer_draw(float delta_time)
{
    // dlear z-buffer
    for (uint32_t i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT; i++)
    {
        z_buffer[i] = FLT_MAX;
    }

    // view matrix
    glm_mat4_identity(view_matrix);
    glm_translate(view_matrix, camera.position);
    glm_quat_rotate(view_matrix, camera.orientation, view_matrix);

    // calculate forward vector
    glm_quat_forward(camera.orientation, forward);

    // rotate mesh
    //rot = rot + 0.03f * (delta_time * (1.0f / FRAMERATE));
    if (rot > GLM_PI * 2)
        rot = 0;

    glm_quatv(mesh->orientation, rot, GLM_YUP);

    // render mesh
    renderer_draw_mesh(mesh);
}

void renderer_shutdown()
{
    free(z_buffer);
    mesh_free(mesh);
}