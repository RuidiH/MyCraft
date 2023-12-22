#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "Component.hpp"
#include <glm/glm.hpp>

class TransformComponent : public Component
{
    public:
        TransformComponent();
        ~TransformComponent();

        void Move(glm::vec3 position);
        void Rotate(glm::vec3 rotation);
        void Scale(glm::vec3 scale);

    private:
        glm::vec3 mPosition;
        glm::vec3 mRotation;
        glm::vec3 mScale;
};


#endif 