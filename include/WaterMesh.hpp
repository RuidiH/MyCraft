// #pragma once

#ifndef WATER_MESH_HPP
#define WATER_MESH_HPP

#include <glm/glm.hpp>
#include <map>
#include <array>

#include "Mesh.hpp"
#include "MeshComponent.hpp"

class MeshComponent;

class WaterMesh : public Mesh
{
public:
    WaterMesh();
    ~WaterMesh();

    void Update() override;
    void Render() override;

    void Init(std::shared_ptr<std::unordered_map<std::string, std::vector<float>>>& vertices) override;
    std::array<glm::vec3, 2> GetCorners() override { return {mMinCorner, mMaxCorner}; };
    glm::vec3 GetSideNormal(std::string side) override;
    
    glm::vec3 GetMinCorner() { return this->mMinCorner; };
    glm::vec3 GetMaxCorner() { return this->mMaxCorner; };

private:
    void SetVertexData();

    const std::vector<GLuint> mIndexBuffer{
        0, 1, 2,
        0, 2, 3};
        
    glm::vec3 mMinCorner;
    glm::vec3 mMaxCorner;
};
#endif