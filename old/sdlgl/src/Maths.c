#include "Maths.h"

void createModelMatrix(Vector3 position, Vector3 rotation, Matrix4 dest)
{
    glm_translate(dest, position);

    glm_rotate_x(dest, glm_rad(rotation[0]), dest);
    glm_rotate_y(dest, glm_rad(rotation[1]), dest);
    glm_rotate_z(dest, glm_rad(rotation[2]), dest);
}

void moveVectorForwards(Vector3 position, Vector3 rotation, float factor)
{
    float yaw = glm_rad(rotation[1]);
    float pitch = glm_rad(rotation[0]);

    position[0] += cosf(yaw) * cosf(pitch) * factor;
    position[1] += sinf(pitch) * factor;
    position[2] += cosf(pitch) * sinf(yaw) * factor;
}

void moveVectorBackwards(Vector3 position, Vector3 rotation, float factor)
{
    float yaw = glm_rad(rotation[1]);
    float pitch = glm_rad(rotation[0]);

    position[0] -= cosf(yaw) * cosf(pitch) * factor;
    position[1] -= sinf(pitch) * factor;
    position[2] -= cosf(pitch) * sinf(yaw) * factor;
}

void moveVectorLeft(Vector3 position, Vector3 rotation, float factor)
{
    float yaw = glm_rad(rotation[1] + 90);

    position[0] += cosf(yaw) * factor;
    position[2] += sinf(yaw) * factor;
}

void moveVectorRight(Vector3 position, Vector3 rotation, float factor)
{
    float yaw = glm_rad(rotation[1] + 90);

    position[0] -= cosf(yaw) * factor;
    position[2] -= sinf(yaw) * factor;
}
