#include "TransformComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <string>

TransformComponent::TransformComponent()
{
    mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    mScale = glm::vec3(1.0f, 1.0f, 1.0f);
}

TransformComponent::~TransformComponent()
{
}

TransformComponent::TransformComponent(glm::vec3 position)
{
    mPosition = position;
    mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    mScale = glm::vec3(1.0f, 1.0f, 1.0f);

    mPositionString = std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);
}

void TransformComponent::Move(glm::vec3 direction)
{
    // currently the object manager's map is not expecting the object to move
    // update the object manager first if this object should be moved.

    // mPosition += direction;
}

void TransformComponent::Rotate(glm::vec3 rotation)
{
    mRotation += rotation;
}

void TransformComponent::Scale(glm::vec3 scale)
{
    mScale += scale;
}

glm::mat4 TransformComponent::GetModelMatrix() const
{
    return glm::translate(mPosition) * glm::rotate(mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(mScale);
}


void TransformComponent::SetPosition(glm::vec3 position) {
    mPosition = position;
    mPositionString = std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);
    std::cout << "transform component position set to " << mPositionString << "\n";
}