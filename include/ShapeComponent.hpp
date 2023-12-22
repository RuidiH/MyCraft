#ifndef SHAPE_COMPONENT_HPP
#define SHAPE_COMPONENT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Component.hpp"
#include "Cube.hpp"

class ShapeComponent : public Component {
    public:
        // Game loop functions
        virtual void Input() override;
        virtual void Update() override;
        virtual void Render() override;

        void setCube(Cube* cube);
    private:
    // TODO: allow a vector of shapes instead of just one
        Cube *cube;                
};


#endif