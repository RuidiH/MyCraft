#include "TransformComponent.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

TransformComponent::TransformComponent()
{
    // mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    // mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    // mScale = glm::vec3(1.0f, 1.0f, 1.0f);
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Move(glm::vec3 position) {
    mPosition = position;
}

void TransformComponent::Rotate(glm::vec3 rotation) {
    mRotation = rotation;
}

void TransformComponent::Scale(glm::vec3 scale) {
    mScale = scale;
}

glm::mat4 TransformComponent::GetModelMatrix() const
{ 
    return glm::translate(mPosition) * glm::rotate(mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(mScale);
}
