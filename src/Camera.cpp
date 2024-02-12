#include "Camera.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

Camera::Camera()
{
}

Camera::Camera(glm::vec3 lookAt, glm::vec3 upVector, glm::vec3 position, glm::vec3 direction)
{
    this->lookAt = lookAt;
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

    direction = glm::rotate(direction, -x * 0.003f, glm::vec3(0.0f, 1.0f, 0.0f));
    direction = glm::rotate(direction, -y * 0.003f, glm::cross(direction, upVector));
}

void Camera::SetLookAt(glm::vec3 lookAt)
{
    this->lookAt = lookAt;
}

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

glm::vec3 Camera::GetLookAt()
{
    return lookAt;
}

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