#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "ShapeComponent.hpp"

class ShapeComponent;

class Cube
{
public:
    Cube();
    Cube(glm::vec3 position, float size);
    // ~Cube();
    void Input();
    void Update();
    void Render();

    glm::vec3 getMinCorner() { return this->mMinCorner; };
    glm::vec3 getMaxCorner() { return this->mMaxCorner; };
    void setPosition(glm::vec3 position);
    void setVertexData();

    void setTexture(GLuint *texId)
    {
        mTexId = texId;
    }

    void setFaceTexture(std::string face, GLuint *texId)
    {
        mTextureIdMap[face] = texId;
    }

    void SetParentComponent(ShapeComponent *parent)
    {
        mParent = parent;
    }

private:
    std::map<std::string, std::vector<float>> mVertexDataMap;
    std::map<std::string, std::vector<GLuint>> mIndexBufferMap;
    std::map<std::string, GLuint *> mTextureIdMap;
    glm::vec3 mPosition;
    float mSize;
    GLuint *mTexId;
    glm::vec3 mMinCorner;
    glm::vec3 mMaxCorner;

    ShapeComponent *mParent;
};

#endif // CUBE_HPP