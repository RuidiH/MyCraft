#include "CubeMesh.hpp"

#include "TransformComponent.hpp"
#include "TextureComponent.hpp"

#include <unordered_set>

// define static values
std::unordered_map<std::string, std::vector<float>> CubeMesh::mVertices;
std::unordered_map<std::string, GLuint> CubeMesh::mVAOs;
std::unordered_map<std::string, GLuint> CubeMesh::mVBOs;
GLuint CubeMesh::mIBO;
bool CubeMesh::mInitialized = false;
const std::vector<GLuint> CubeMesh::mIndexBuffer{
    0, 1, 2,
    0, 2, 3};

CubeMesh::CubeMesh()
{
    mSize = 1.0f;
}

void CubeMesh::Init()
{
    // exit if parent does not have a transform component
    if (!mParent || !mParent->GetParent()->HasComponent<TransformComponent>())
    {
        std::cout << "Cube Mesh component's parent does not have a transform component\n";
        exit(1);
    }

    glm::vec3 position = mParent->GetParent()->GetComponent<TransformComponent>()->GetPosition();
    mMinCorner = position - glm::vec3(mSize / 2.0);
    mMaxCorner = position + glm::vec3(mSize / 2.0);
}

CubeMesh::~CubeMesh()
{
    for (const auto side : mParent->GetVisibleSides())
    {
        OffloadFace(side);
    }
}

void CubeMesh::Update()
{
}

void CubeMesh::Render()
{
    // initialize static variables if not already
    if (!mInitialized)
    {
        SetVertexData();
    }

    if (!mTextureIdMap)
    {
        mTextureIdMap = std::make_shared<std::unordered_map<std::string, GLuint *>>();
    }

    if (mTextureIdMap->empty())
    {
        for (const auto &texId : mParent->GetParent()->GetComponent<TextureComponent>()->GetTextureGroup())
        {
            mTextureIdMap->insert(texId);
        }
    }

    for (const auto face : mParent->GetVisibleSides())
    {
        GLuint *currentTexID = mTextureIdMap.get()->at(face);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *currentTexID);

        glBindVertexArray(mVAOs.at(face));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
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

void CubeMesh::SetVertexData()
{
    if (mInitialized)
    {
        return;
    }

    float radius = mSize / 2.0f;

    std::vector<float> sTop{
        // Top
        -radius, radius, -radius, 0.0f, 0.0f, 0.f, 1.f, 0.f, // - + -
        -radius, radius, radius, 0.0f, 1.0f, 0.f, 1.f, 0.f,  // - + +
        radius, radius, radius, 1.0f, 1.0f, 0.f, 1.f, 0.f,   // + + +
        radius, radius, -radius, 1.0f, 0.0f, 0.f, 1.f, 0.f   // + + -
    };

    std::vector<float> sBottom{
        // Bottom
        -radius, -radius, radius, 0.0f, 1.0f, 0.f, -1.f, 0.f,  // - - +
        -radius, -radius, -radius, 0.0f, 0.0f, 0.f, -1.f, 0.f, // - - -
        radius, -radius, -radius, 1.0f, 0.0f, 0.f, -1.f, 0.f,  // + - -
        radius, -radius, radius, 1.0f, 1.0f, 0.f, -1.f, 0.f    // + - +
    };

    std::vector<float> sLeft{
        // Left
        -radius, radius, -radius, 0.0f, 0.0f, -1.f, 0.f, 0.f,  // - + -
        -radius, -radius, -radius, 0.0f, 1.0f, -1.f, 0.f, 0.f, // - - -
        -radius, -radius, radius, 1.0f, 1.0f, -1.f, 0.f, 0.f,  // - - +
        -radius, radius, radius, 1.0f, 0.0f, -1.f, 0.f, 0.f    // - + +
    };

    std::vector<float> sRight{
        // Right
        radius, radius, radius, 0.0f, 0.0f, 1.f, 0.f, 0.f,   // + + +
        radius, -radius, radius, 0.0f, 1.0f, 1.f, 0.f, 0.f,  // + - +
        radius, -radius, -radius, 1.0f, 1.0f, 1.f, 0.f, 0.f, // + - -
        radius, radius, -radius, 1.0f, 0.0f, 1.f, 0.f, 0.f   // + + -
    };

    std::vector<float> sFront{
        // Front
        -radius, radius, radius, 0.0f, 0.0f, 0.f, 0.f, 1.f,  // - + +
        -radius, -radius, radius, 0.0f, 1.0f, 0.f, 0.f, 1.f, // - - +
        radius, -radius, radius, 1.0f, 1.0f, 0.f, 0.f, 1.f,  // + - +
        radius, radius, radius, 1.0f, 0.0f, 0.f, 0.f, 1.f    // + + +
    };

    std::vector<float> sBack{
        // Back
        -radius, -radius, -radius, 1.0f, 1.0f, 0.f, 0.f, -1.f, // - - -
        -radius, radius, -radius, 1.0f, 0.0f, 0.f, 0.f, -1.f,  // - + -
        radius, radius, -radius, 0.0f, 0.0f, 0.f, 0.f, -1.f,   // + + -
        radius, -radius, -radius, 0.0f, 1.0f, 0.f, 0.f, -1.f   // + - -
    };

    mVertices.insert({"top", sTop});
    mVertices.insert({"bottom", sBottom});
    mVertices.insert({"left", sLeft});
    mVertices.insert({"right", sRight});
    mVertices.insert({"front", sFront});
    mVertices.insert({"back", sBack});

    std::vector<std::string> sides = {"top", "bottom", "left", "right", "front", "back"};

    for (const auto &side : sides)
    {
        LoadFace(side);
    }

    mInitialized = true;
}

void CubeMesh::LoadFace(std::string face)
{
    GLuint vao, vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

    // set index buffer data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mIndexBuffer.size() * sizeof(GLuint),
                 mIndexBuffer.data(),
                 GL_STATIC_DRAW);

    // set vertex data
    glBufferData(GL_ARRAY_BUFFER,
                 mVertices.at(face).size() * sizeof(GL_FLOAT),
                 mVertices.at(face).data(),
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
    mVAOs.insert({face, vao});
    mVBOs.insert({face, vbo});
}

void CubeMesh::OffloadFace(std::string face)
{
    glDeleteVertexArrays(1, &mVAOs.at(face));
    glDeleteBuffers(1, &mVBOs.at(face));
    glDeleteBuffers(1, &mIBO);
    mParent->RemoveVisibleSide(face);
}