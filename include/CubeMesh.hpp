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

    // void SetTexture(GLuint *texId) { mTexId = texId; }
    // void SetFaceTexture(std::string face, GLuint *texId) { mTextureIdMap[face] = texId; }
    // void SetFaceTexture(const std::map<std::string, GLuint *> &textureIdMap) { mTextureIdMap = textureIdMap; }
    // std::map<std::string, GLuint *> GetTextureIdMap() { return mTextureIdMap; };

    glm::vec3 GetMinCorner() { return this->mMinCorner; };
    glm::vec3 GetMaxCorner() { return this->mMaxCorner; };
    std::array<glm::vec3, 2> GetCorners() { return {mMinCorner, mMaxCorner}; };
    glm::vec3 GetSideNormal(std::string side);


private:
    void SetVertexData();

    std::map<std::string, std::vector<float>> mVertexDataMap;

    // std::map<std::string, std::vector<GLuint>> mIndexBufferMap;
    // std::map<std::string, GLuint *> mTextureIdMap;
    // GLuint *mTexId;

    glm::vec3 mMinCorner;
    glm::vec3 mMaxCorner;
};