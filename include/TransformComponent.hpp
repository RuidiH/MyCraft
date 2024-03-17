#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "Component.hpp"
#include <glm/glm.hpp>

class TransformComponent : public Component
{
    public:
        TransformComponent();
        TransformComponent(glm::vec3 position);
        ~TransformComponent();

        void SetPosition(glm::vec3 position) { mPosition = position; }
        void SetRotation(glm::vec3 rotation) { mRotation = rotation; }
        void SetScale(glm::vec3 scale) { mScale = scale; }


        void Move(glm::vec3 direction);
        void Rotate(glm::vec3 rotation);
        void Scale(glm::vec3 scale);

        glm::vec3 GetPosition() const { return mPosition; }
        glm::vec3 GetRotation() const { return mRotation; } 
        glm::vec3 GetScale() const { return mScale; }
        glm::mat4 GetModelMatrix() const;
    private:
        glm::vec3 mPosition;
        glm::vec3 mRotation;
        glm::vec3 mScale;
};


#endif 