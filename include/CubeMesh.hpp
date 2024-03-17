#pragma once

// #include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <array>
#include <iostream>

#include "Mesh.hpp"
#include "MeshComponent.hpp"

class MeshComponent;

class CubeMesh : public Mesh
{
public:
    CubeMesh();
    ~CubeMesh(){};

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