#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "Component.hpp"
#include "CubeMesh.hpp"
#include "Mesh.hpp"

enum class MeshType {
    CUBE,
    WATER 
};

class Cube;
class CubeMesh;
class Mesh;
class MeshComponent : public Component {
    public:
        // Game loop functions
        virtual void Input() override;
        virtual void Update() override;
        virtual void Render() override;

        std::shared_ptr<Mesh> AddMesh(MeshType type);
        std::shared_ptr<Mesh> GetMesh() {return mMesh;};

        MeshType GetMeshType() {return mMeshType;};
    private:
        // Cube *cube;                
        // std::vector<std::shared_ptr<Cube>> mCubes;
        std::shared_ptr<Mesh> mMesh;
        MeshType mMeshType; 
};


#endif