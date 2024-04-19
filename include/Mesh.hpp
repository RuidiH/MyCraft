#pragma once

#include <glm/glm.hpp>

#include "MeshComponent.hpp"

#include <map>
#include <unordered_map>
#include <string>
#include <array>
#include <memory>

class MeshComponent;

class Mesh {
public:
    Mesh() {};
    ~Mesh() {};
    virtual void Update() = 0;
    virtual void Render() = 0;

    virtual void Init() = 0;
    virtual glm::vec3 GetSideNormal(std::string side) = 0;
    virtual std::array<glm::vec3, 2> GetCorners() = 0;


    void SetParentComponent(MeshComponent *parent) { mParent = parent; }

protected:
    virtual void LoadFace(std::string face) = 0;
    virtual void OffloadFace(std::string face) = 0; 

    float mSize = 1.0f;
    MeshComponent *mParent;
    // std::shared_ptr<std::unordered_map<std::string, std::vector<float>>> mVertexDataMap;
    std::map<std::string, std::array<GLuint, 3>> mBufferObjectsMap; // vao, vbo, ibo
};