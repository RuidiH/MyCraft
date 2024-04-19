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

    void Init() override;
    std::array<glm::vec3, 2> GetCorners() override { return {mMinCorner, mMaxCorner}; };
    glm::vec3 GetSideNormal(std::string side) override;

    glm::vec3 GetMinCorner() { return this->mMinCorner; };
    glm::vec3 GetMaxCorner() { return this->mMaxCorner; };
    // void SetFaceInWater(std::string face, bool inWater) { isFaceInWater[face] = inWater; };

private:
    void SetVertexData();
    
    void LoadFace(std::string face) override;
    void OffloadFace(std::string face) override;

    // std::unordered_map<std::string, bool> isFaceInWater;
    glm::vec3 mMinCorner;
    glm::vec3 mMaxCorner;
    std::shared_ptr<std::unordered_map<std::string, GLuint *>> mTextureIdMap;

    // static values
    static std::unordered_map<std::string, std::vector<float>> mVertices;
    static std::unordered_map<std::string, GLuint> mVAOs;
    static std::unordered_map<std::string, GLuint> mVBOs;
    static GLuint mIBO;
    static const std::vector<GLuint> mIndexBuffer;
    static bool mInitialized;
};

#endif