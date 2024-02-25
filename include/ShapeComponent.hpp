#ifndef SHAPE_COMPONENT_HPP
#define SHAPE_COMPONENT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "Component.hpp"
#include "Cube.hpp"

class Cube;
class ShapeComponent : public Component {
    public:
        // Game loop functions
        virtual void Input() override;
        virtual void Update() override;
        virtual void Render() override;

        void AddCube(std::shared_ptr<Cube> cube);
        std::shared_ptr<Cube> AddCube();
        std::vector<std::shared_ptr<Cube>> GetCubes() {return mCubes;};
    private:
        // Cube *cube;                
        std::vector<std::shared_ptr<Cube>> mCubes;
 
};


#endif