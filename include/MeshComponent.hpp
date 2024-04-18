#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>

#include "Mesh.hpp"
#include "Component.hpp"

class Mesh;
class CubeMesh;
class WaterMesh;

enum class MeshType {
    CUBE,
    WATER 
};

class MeshComponent : public Component {
    public:
        MeshComponent(MeshType type);

        // Game loop functions
        virtual void Input() override;
        virtual void Update() override;
        virtual void Render() override;

        // std::shared_ptr<Mesh> AddMesh(MeshType type);
        void Init();
        std::shared_ptr<Mesh> GetMesh() {return mMesh;};

        MeshType GetMeshType() {return mMeshType;};
        void SetVisibility(std::string side, bool visible);
        bool GetVisibility(std::string side) {return mVisibilityMap[side];};

        std::shared_ptr<std::unordered_set<std::string>> GetVisibleSides() {return mVisibleSides;}; 
        void AddVisibleSide(std::string side) {mVisibleSides->insert(side);};
        void RemoveVisibleSide(std::string side) {mVisibleSides->erase(side);};

    private:
        std::shared_ptr<Mesh> mMesh;
        MeshType mMeshType; 
        std::map<std::string, bool> mVisibilityMap;
        std::shared_ptr<std::unordered_set<std::string>> mVisibleSides;
};


#endif