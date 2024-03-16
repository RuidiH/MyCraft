#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <array>


#include "MeshComponent.hpp"

class MeshComponent;

class Cube
{
public:
    Cube();
    Cube(glm::vec3 position, float size);
    // ~Cube();
    void Input();
    void Update();
    void Render();

    glm::vec3 GetSideNormal(std::string side);
    glm::vec3 GetMinCorner() { return this->mMinCorner; };
    glm::vec3 GetMaxCorner() { return this->mMaxCorner; };
    std::array<glm::vec3, 2> GetCorners() { return {mMinCorner, mMaxCorner}; };
    std::map<std::string, GLuint *> GetTextureIdMap() { return mTextureIdMap; };

    void setPosition(glm::vec3 position);
    void SetVertexData();
    void SetTexture(GLuint *texId) { mTexId = texId; }

    void SetFaceTexture(std::string face, GLuint *texId) { mTextureIdMap[face] = texId; }
    void SetFaceTexture(const std::map<std::string, GLuint *> &textureIdMap) { mTextureIdMap = textureIdMap; }

    void SetParentComponent(MeshComponent *parent) { mParent = parent; }

private:
    std::map<std::string, std::vector<float>> mVertexDataMap;
    // std::map<std::string, std::vector<GLuint>> mIndexBufferMap;
    std::map<std::string, GLuint *> mTextureIdMap;
    // glm::vec3 mPosition;
    float mSize;
    GLuint *mTexId;
    glm::vec3 mMinCorner;
    glm::vec3 mMaxCorner;

    MeshComponent *mParent;
};

#endif // CUBE_HPP