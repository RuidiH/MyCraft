// #pragma once

#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP

// #include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <array>
#include <iostream>

#include "Mesh.hpp"
#include "MeshComponent.hpp"

class MeshComponent;
class Mesh;

class CubeMesh : public Mesh
{
public:
    CubeMesh();
    ~CubeMesh();

    void Update() override;
    void Render() override;

    void Init(std::shared_ptr<std::unordered_map<std::string, std::vector<float>>>& vertices) override;
    glm::vec3 GetMinCorner() { return this->mMinCorner; };
    glm::vec3 GetMaxCorner() { return this->mMaxCorner; };
    std::array<glm::vec3, 2> GetCorners() override { return {mMinCorner, mMaxCorner}; };
    glm::vec3 GetSideNormal(std::string side) override;
    void SetFaceInWater(std::string face, bool inWater) { isFaceInWater[face] = inWater; };

private:
    void SetVertexData();

    const std::vector<GLuint> mIndexBuffer{
        0, 1, 2,
        0, 2, 3};

    std::unordered_map<std::string, bool> isFaceInWater;
    glm::vec3 mMinCorner;
    glm::vec3 mMaxCorner;
    bool mIsPositionUpdated = false;
    std::shared_ptr<std::unordered_map<std::string, GLuint *>> mTextureIdMap;
};

#endif