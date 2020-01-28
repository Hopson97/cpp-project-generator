#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <array>

glm::mat4 createProjectionViewMatrix(const glm::vec3 &position,
                                     const glm::vec3 &rotation,
                                     const glm::mat4 &projection);

/**
 * @brief Rotate a matrix
 *
 * @param matrix The matrix to rotate
 * @param degrees Rotation axis in degrees
 */
void rotateMatrix(glm::mat4 &matrix, const glm::vec3 &degrees);

/**
 * @brief Translates a matrix
 *
 * @param matrix The matrix to translate
 * @param offset The amount of translate by
 */
void translateMatrix(glm::mat4 &matrix, const glm::vec3 &offset);

/**
 * @brief Scales a matrix
 *
 * @param matrix The matrix to scale
 * @param scalars The amount to scale each axis by
 */
void scaleMatrix(glm::mat4 &matrix, const glm::vec3 &scalars);

/**
 * @brief Scales a matrix
 *
 * @param matrix The matrix to scale
 * @param scalar The amount to scale the whole matrix by
 */
void scaleMatrix(glm::mat4 &matrix, float scalar);

/**
 * @brief Creates a forwards-facing vector
 *
 * @param rotation The rotation axis to base the result on
 * @return glm::vec3 A vector that is forwards from the rotation vector
 */
glm::vec3 forwardsVector(const glm::vec3 &rotation);

/**
 * @brief Creates a backwards-facing vector
 *
 * @param rotation The rotation axis to base the result on
 * @return glm::vec3 A vector that is backwards from the rotation vector
 */
glm::vec3 backwardsVector(const glm::vec3 &rotation);

/**
 * @brief Creates a left-facing vector
 *
 * @param rotation The rotation axis to base the result on
 * @return glm::vec3 A vector that is left from the rotation vector
 */
glm::vec3 leftVector(const glm::vec3 &rotation);

/**
 * @brief Creates a right-facing vector
 *
 * @param rotation The rotation axis to base the result on
 * @return glm::vec3 A vector that is right from the rotation vector
 */
glm::vec3 rightVector(const glm::vec3 &rotation);