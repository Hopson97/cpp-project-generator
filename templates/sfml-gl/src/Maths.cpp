#include "Maths.h"

glm::mat4 createModelMatrix(const Transform& transform)
{
    glm::mat4 matrix{1.0f};

    matrix = glm::translate(matrix, transform.position);

    matrix = glm::rotate(matrix, glm::radians(transform.rotation.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(transform.rotation.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(transform.rotation.z), {0, 0, 1});
    return matrix;
}

glm::mat4 createOrbitModelMatrix(const Transform& transform)
{
    glm::mat4 matrix{1.0f};

    matrix = glm::rotate(matrix, glm::radians(transform.rotation.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(transform.rotation.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(transform.rotation.z), {0, 0, 1});

    matrix = glm::translate(matrix, transform.position);

    return matrix;
}

glm::mat4 createViewMartix(const Transform& transform, const glm::vec3& up)
{
    glm::vec3 center{0.0f};
    glm::vec3 front{0.0f};

    front.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    front.y = sin(glm::radians(transform.rotation.x));
    front.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    front = glm::normalize(front);

    center = transform.position + front;

    return glm::lookAt(transform.position, center, up);
}

glm::mat4 createProjectionMatrix(float aspectRatio, float fov)
{
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 10000.0f);
}

glm::vec3 forwardsVector(const glm::vec3& rotation)
{
    float yaw = glm::radians(rotation.y);
    float pitch = glm::radians(rotation.x);

    return {glm::cos(yaw) * glm::cos(pitch), glm::sin(pitch), glm::cos(pitch) * glm::sin(yaw)};
}

glm::vec3 backwardsVector(const glm::vec3& rotation)
{
    return -forwardsVector(rotation);
}

glm::vec3 leftVector(const glm::vec3& rotation)
{
    float yaw = glm::radians(rotation.y + 90);
    return {-glm::cos(yaw), 0, -glm::sin(yaw)};
}

glm::vec3 rightVector(const glm::vec3& rotation)
{
    return -leftVector(rotation);
}

// https://web.archive.org/web/20160327184424/http://ruh.li/CameraViewFrustum.html
enum Planes {
    Near,
    Far,
    Left,
    Right,
    Top,
    Bottom,
};

float ViewFrustum::Plane::distanceToPoint(const glm::vec3& point) const
{
    return glm::dot(point, normal) + distanceToOrigin;
}

void ViewFrustum::update(const glm::mat4& mat) noexcept
{
    // Left
    m_planes[Planes::Left].normal.x = mat[0][3] + mat[0][0];
    m_planes[Planes::Left].normal.y = mat[1][3] + mat[1][0];
    m_planes[Planes::Left].normal.z = mat[2][3] + mat[2][0];
    m_planes[Planes::Left].distanceToOrigin = mat[3][3] + mat[3][0];

    // Right
    m_planes[Planes::Right].normal.x = mat[0][3] - mat[0][0];
    m_planes[Planes::Right].normal.y = mat[1][3] - mat[1][0];
    m_planes[Planes::Right].normal.z = mat[2][3] - mat[2][0];
    m_planes[Planes::Right].distanceToOrigin = mat[3][3] - mat[3][0];

    // Bottom
    m_planes[Planes::Bottom].normal.x = mat[0][3] + mat[0][1];
    m_planes[Planes::Bottom].normal.y = mat[1][3] + mat[1][1];
    m_planes[Planes::Bottom].normal.z = mat[2][3] + mat[2][1];
    m_planes[Planes::Bottom].distanceToOrigin = mat[3][3] + mat[3][1];

    // Top
    m_planes[Planes::Top].normal.x = mat[0][3] - mat[0][1];
    m_planes[Planes::Top].normal.y = mat[1][3] - mat[1][1];
    m_planes[Planes::Top].normal.z = mat[2][3] - mat[2][1];
    m_planes[Planes::Top].distanceToOrigin = mat[3][3] - mat[3][1];

    // Near
    m_planes[Planes::Near].normal.x = mat[0][3] + mat[0][2];
    m_planes[Planes::Near].normal.y = mat[1][3] + mat[1][2];
    m_planes[Planes::Near].normal.z = mat[2][3] + mat[2][2];
    m_planes[Planes::Near].distanceToOrigin = mat[3][3] + mat[3][2];

    // Far
    m_planes[Planes::Far].normal.x = mat[0][3] - mat[0][2];
    m_planes[Planes::Far].normal.y = mat[1][3] - mat[1][2];
    m_planes[Planes::Far].normal.z = mat[2][3] - mat[2][2];
    m_planes[Planes::Far].distanceToOrigin = mat[3][3] - mat[3][2];

    for (auto& plane : m_planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distanceToOrigin /= length;
    }
}