#pragma once

#include <cglm/struct.h>
#include <math.h>

typedef mat4 Matrix4;
typedef vec3 Vector3;

#define VECTOR3_ZERO GLM_VEC3_ZERO_INIT
#define MATRIX4_IDENTITY GLM_MAT4_IDENTITY_INIT

void createModelMatrix(Vector3 position, Vector3 rotation, Matrix4 dest);

void moveVectorForwards(Vector3 position, Vector3 rotation, float factor);
void moveVectorBackwards(Vector3 position, Vector3 rotation, float factor);
void moveVectorLeft(Vector3 position, Vector3 rotation, float factor);
void moveVectorRight(Vector3 position, Vector3 rotation, float factor);
