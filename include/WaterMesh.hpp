// #pragma once

#ifndef WATER_MESH_HPP
#define WATER_MESH_HPP

#include <glm/glm.hpp>

#include "Mesh.hpp"
#include "MeshComponent.hpp"

class MeshComponent;

class WaterMesh : public Mesh
{
public:
    WaterMesh();
    ~WaterMesh(){};

    void Update() override;
    void Render() override;

    glm::vec3 GetMinCorner() { return this->mMinCorner; };
    glm::vec3 GetMaxCorner() { return this->mMaxCorner; };
    std::array<glm::vec3, 2> GetCorners() { return {mMinCorner, mMaxCorner}; };
    glm::vec3 GetSideNormal(std::string side) override;

private:
    void SetVertexData();

    std::map<std::string, std::vector<float>> mVertexDataMap;

    glm::vec3 mMinCorner;
    glm::vec3 mMaxCorner;
};
#endif