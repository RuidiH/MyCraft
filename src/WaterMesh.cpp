#include "WaterMesh.hpp"
#include "TransformComponent.hpp"

#include <iostream>

WaterMesh::WaterMesh()
{
    mSize = 1.0f;
    mVertexDataMap = std::make_shared<std::unordered_map<std::string, std::vector<float>>>();
}

void WaterMesh::Init(std::shared_ptr<std::unordered_map<std::string, std::vector<float>>> &vertices)
{
    mVisibleSides = mParent->GetVisibleSides();

    // exit if parent does not have a transform component
    if (!mParent || !mParent->GetParent()->HasComponent<TransformComponent>())
    {
        std::cout << "Water Mesh component's parent does not have a transform component\n";
        exit(1);
    }

    glm::vec3 position = mParent->GetParent()->GetComponent<TransformComponent>()->GetPosition();
    mMinCorner = position - glm::vec3(mSize / 2.0);
    mMaxCorner = position + glm::vec3(mSize / 2.0);
    mVertexDataMap = vertices;
}

WaterMesh::~WaterMesh()
{
    for (const auto &pair : mBufferObjectsMap)
    {
        glDeleteBuffers(1, &pair.second.at(0));
        glDeleteBuffers(1, &pair.second.at(1));
        glDeleteVertexArrays(1, &pair.second.at(2));
    }
}

void WaterMesh::Update()
{
}

void WaterMesh::Render()
{
    for (const auto &face : *mVisibleSides)
    {
        std::array<GLuint, 3> buffers = mBufferObjectsMap[face];

        glBindVertexArray(buffers.at(0));
        glBindBuffer(GL_ARRAY_BUFFER, buffers.at(1));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.at(2));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

void WaterMesh::SetVertexData()
{
}

glm::vec3 WaterMesh::GetSideNormal(std::string side)
{
    if (side == "top")
    {
        return glm::vec3(0.f, 1.f, 0.f);
    }
    else if (side == "bottom")
    {
        return glm::vec3(0.f, -1.f, 0.f);
    }
    else if (side == "left")
    {
        return glm::vec3(-1.f, 0.f, 0.f);
    }
    else if (side == "right")
    {
        return glm::vec3(1.f, 0.f, 0.f);
    }
    else if (side == "front")
    {
        return glm::vec3(0.f, 0.f, 1.f);
    }
    else if (side == "back")
    {
        return glm::vec3(0.f, 0.f, -1.f);
    }
    std::cout << "Invalid side\n";
    return glm::vec3(0.f, 0.f, 0.f);
}

void WaterMesh::LoadFace(std::string face)
{
    std::array<GLuint, 3> buffers;

    glGenVertexArrays(1, &buffers.at(0));
    glBindVertexArray(buffers.at(0));

    glGenBuffers(1, &buffers.at(1));
    glBindBuffer(GL_ARRAY_BUFFER, buffers.at(1));

    glGenBuffers(1, &buffers.at(2));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.at(2));

    // set vertex data
    glBufferData(GL_ARRAY_BUFFER,
                 mVertexDataMap->at(face).size() * sizeof(GL_FLOAT),
                 mVertexDataMap->at(face).data(),
                 GL_STATIC_DRAW);

    // set index buffer data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mIndexBuffer.size() * sizeof(GLuint),
                 mIndexBuffer.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3, // x, y, z
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 9,
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3, // r, g, b
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 9,
                          (GLvoid *)(sizeof(GL_FLOAT) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          3, // normal
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 9,
                          (GLvoid *)(sizeof(GL_FLOAT) * 6));

    // unbind
    glBindVertexArray(0);
    mBufferObjectsMap[face] = buffers;
}

void WaterMesh::OffloadFace(std::string face)
{
    std::array<GLuint, 3> buffers = mBufferObjectsMap[face];
    glDeleteBuffers(1, &buffers.at(0));
    glDeleteBuffers(1, &buffers.at(1));
    glDeleteVertexArrays(1, &buffers.at(2));
    mVisibleSides->erase(face);
    mBufferObjectsMap.erase(face);
}