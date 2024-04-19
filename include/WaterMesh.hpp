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

    void Init() override;
    std::array<glm::vec3, 2> GetCorners() override { return {mMinCorner, mMaxCorner}; };
    glm::vec3 GetSideNormal(std::string side) override;

    glm::vec3 GetMinCorner() { return this->mMinCorner; };
    glm::vec3 GetMaxCorner() { return this->mMaxCorner; };

private:
    void SetVertexData();
    
    void LoadFace(std::string face) override; 
    void OffloadFace(std::string face) override;
 
    glm::vec3 mMinCorner;
    glm::vec3 mMaxCorner;

    // static values
    static std::unordered_map<std::string, std::vector<float>> mVertices;
    static std::unordered_map<std::string, GLuint> mVAOs;
    static std::unordered_map<std::string, GLuint> mVBOs;
    static GLuint mIBO;
    static const std::vector<GLuint> mIndexBuffer;
    static bool mInitialized;
};
#endif