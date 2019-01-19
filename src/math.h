#ifndef MATH_H
#define MATH_H

#include <cglm/cglm.h>

void glm_mat4_project(vec3 v, mat4 m, vec3 dest);

void glm_quat_forward(versor q, vec3 v);

#endif