#include "Camera.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

Camera::Camera()
{
}

Camera::Camera(glm::vec3 upVector, glm::vec3 position, glm::vec3 direction)
{
    this->upVector = upVector;
    this->position = position;
    this->direction = direction;
}

Camera::~Camera()
{
}

void Camera::MoveForward(float speed)
{
    position += direction * speed;
}

void Camera::MoveBackward(float speed)
{
    position -= direction * speed;
}

void Camera::MoveLeft(float speed)
{
    position -= glm::normalize(glm::cross(direction, upVector)) * speed;
}

void Camera::MoveRight(float speed)
{
    position += glm::normalize(glm::cross(direction, upVector)) * speed;
}

void Camera::RotateCamera(float x, float y)
{
    direction = glm::rotate(direction, glm::radians(-x * 0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
    direction = glm::rotate(direction, glm::radians(-y * 0.05f), glm::cross(direction, upVector));
}

void Camera::UpdateViewMatrix()
{
    viewMatrix = glm::lookAt(position, position + direction, upVector);
}

// void Camera::SetLookAt(glm::vec3 lookAt)
// {
//     this->lookAt = lookAt;
// }

void Camera::SetUpVector(glm::vec3 upVector)
{
    this->upVector = upVector;
}

void Camera::SetPosition(glm::vec3 position)
{
    this->position = position;
}

void Camera::SetDirection(glm::vec3 direction)
{
    this->direction = direction;
}

void Camera::SetProjectionMatrix(float fov, float aspect, float near, float far)
{
    projectionMatrix = glm::perspective(fov, aspect, near, far);
}

// glm::vec3 Camera::GetLookAt()
// {
//     return lookAt;
// }

glm::vec3 Camera::GetUpVector()
{
    return upVector;
}

glm::vec3 Camera::GetPosition() {
    return position;
}

glm::vec3 Camera::GetDirection() {
    return direction;
}

glm::mat4 Camera::GetViewMatrix() {
    return viewMatrix;
}   

glm::mat4 Camera::GetProjectionMatrix() {
    return projectionMatrix;
}
glm::vec3 Camera::GetRayDirection(int screenWidth, int screenHeight, glm::vec2 rayStart) {
    float x = (2.0f * rayStart.x / screenWidth) - 1.0f;
    float y = 1.0f - (2.0f * rayStart.y / screenHeight);
    glm::vec4 rayClip = glm::vec4(x, y, -1.0, 1.0);

    glm::vec4 rayEye = inverse(projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    glm::vec3 rayWorld = glm::vec3(inverse(viewMatrix) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    return rayWorld;
}
