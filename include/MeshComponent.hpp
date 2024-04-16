#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "Component.hpp"
#include "Mesh.hpp"

class Mesh;
class CubeMesh;
// class WaterMesh;

enum class MeshType {
    CUBE,
    WATER 
};

class MeshComponent : public Component {
    public:
        // Game loop functions
        virtual void Input() override;
        virtual void Update() override;
        virtual void Render() override;

        std::shared_ptr<Mesh> AddMesh(MeshType type);
        std::shared_ptr<Mesh> GetMesh() {return mMesh;};

        MeshType GetMeshType() {return mMeshType;};
        void SetVisibility(std::string side, bool visible);
        bool GetVisibility(std::string side) {return mVisibilityMap[side];};
    
    private:
        std::shared_ptr<Mesh> mMesh;
        MeshType mMeshType; 
        std::map<std::string, bool> mVisibilityMap;
};


#endif