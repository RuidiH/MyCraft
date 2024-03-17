#pragma once

#include "MeshComponent.hpp"

#include <glm/glm.hpp>

class MeshComponent;

class Mesh {
public:
    Mesh() {};
    ~Mesh() {};
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual glm::vec3 GetSideNormal(std::string side) = 0;
    void SetParentComponent(MeshComponent *parent) { mParent = parent; }
protected:
    float mSize = 1.0f;
    MeshComponent *mParent;
};

