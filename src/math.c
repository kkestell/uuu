#include <cglm/cglm.h>

#include "config.h"

void glm_mat4_project(vec3 v, mat4 m, vec3 dest)
{
    vec4 p;
    glm_vec4(v, 1, p);

    glm_mat4_mulv(m, p, p);
    
    glm_vec4_divs(p, p[3], p);

    p[0] =  p[0] * FRAMEBUFFER_WIDTH  + (float)FRAMEBUFFER_WIDTH  / 2.0f;
    p[1] = -p[1] * FRAMEBUFFER_HEIGHT + (float)FRAMEBUFFER_HEIGHT / 2.0f;

    glm_vec3(p, dest);
}

void glm_quat_forward(versor q, vec3 v) 
{
  float x2  = 2.0f * q[0];
  float y2  = 2.0f * q[1];
  float z2  = 2.0f * q[2];
  float x2w = x2   * q[3];
  float y2w = y2   * q[3];
  float x2x = x2   * q[0];
  float z2x = z2   * q[0];
  float y2y = y2   * q[1];
  float z2y = z2   * q[1];
  
  v[0] = z2x + y2w;
  v[1] = z2y - x2w;
  v[2] = 1.0f - (x2x + y2y);
}