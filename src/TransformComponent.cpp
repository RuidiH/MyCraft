#include "TransformComponent.hpp"

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

