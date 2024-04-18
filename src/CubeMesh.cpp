#include "CubeMesh.hpp"

#include "TransformComponent.hpp"
#include "TextureComponent.hpp"

#include <unordered_set>

CubeMesh::CubeMesh()
{
    mSize = 1.0f;
    mVertexDataMap = std::make_shared<std::unordered_map<std::string, std::vector<float>>>();
}

void CubeMesh::Init(std::shared_ptr<std::unordered_map<std::string, std::vector<float>>> &vertices)
{
    mVisibleSides = mParent->GetVisibleSides();
    mTextureIdMap = std::make_shared<std::unordered_map<std::string, GLuint *>>();

    // exit if parent does not have a transform component
    if (!mParent || !mParent->GetParent()->HasComponent<TransformComponent>())
    {
        std::cout << "Cube Mesh component's parent does not have a transform component\n";
        exit(1);
    }

    // SetVertexData();
    glm::vec3 position = mParent->GetParent()->GetComponent<TransformComponent>()->GetPosition();
    mMinCorner = position - glm::vec3(mSize / 2.0);
    mMaxCorner = position + glm::vec3(mSize / 2.0);
    mVertexDataMap = vertices;
}

CubeMesh::~CubeMesh()
{
    // for (const auto &pair : mBufferObjectsMap)
    // {
    //     glDeleteBuffers(1, &pair.second.at(0));
    //     glDeleteBuffers(1, &pair.second.at(1));
    //     glDeleteVertexArrays(1, &pair.second.at(2));
    // }

    for (const auto side : *mVisibleSides)
    {
        OffloadFace(side);
    }
}

void CubeMesh::Update()
{
}

void CubeMesh::Render()
{
    if (mTextureIdMap->empty())
    {
        for (const auto &texId : mParent->GetParent()->GetComponent<TextureComponent>()->GetTextureGroup())
        {
            mTextureIdMap->insert(texId);
        }
    }

    for (const auto &face : *mVisibleSides)
    {
        if (mBufferObjectsMap.find(face) == mBufferObjectsMap.end())
        {
            LoadFace(face);
        }
        std::array<GLuint, 3> buffers = mBufferObjectsMap.at(face);
        GLuint *currentTexID = mTextureIdMap.get()->at(face);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *currentTexID);
        glBindVertexArray(buffers.at(0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void CubeMesh::SetVertexData()
{
}

glm::vec3 CubeMesh::GetSideNormal(std::string side)
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

void CubeMesh::LoadFace(std::string face)
{
    if (mBufferObjectsMap.find(face) != mBufferObjectsMap.end())
    {
        return;
    }

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
                          sizeof(GL_FLOAT) * 8,
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2, // u, v
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 8,
                          (GLvoid *)(sizeof(GL_FLOAT) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          3, // nx, ny, nz
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 8,
                          (GLvoid *)(sizeof(GL_FLOAT) * 5));

    // unbind
    glBindVertexArray(0);
    // mVisibleSides->insert(face);
    mBufferObjectsMap.insert({face, buffers});
}

void CubeMesh::OffloadFace(std::string face)
{
    if (mBufferObjectsMap.find(face) == mBufferObjectsMap.end())
    {
        return;
    }
    std::array<GLuint, 3> buffers = mBufferObjectsMap.at(face);
    glDeleteVertexArrays(1, &buffers.at(0));
    glDeleteBuffers(1, &buffers.at(1));
    glDeleteBuffers(1, &buffers.at(2));
    mVisibleSides->erase(face);
    mBufferObjectsMap.erase(face);
}