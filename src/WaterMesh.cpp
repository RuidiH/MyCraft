#include "WaterMesh.hpp"
#include "TransformComponent.hpp"

#include <iostream>

// declare static variables
std::unordered_map<std::string, std::vector<float>> WaterMesh::mVertices;
std::unordered_map<std::string, GLuint> WaterMesh::mVAOs;
std::unordered_map<std::string, GLuint> WaterMesh::mVBOs;
GLuint WaterMesh::mIBO;
bool WaterMesh::mInitialized = false;
const std::vector<GLuint> WaterMesh::mIndexBuffer{
    0, 1, 2,
    0, 2, 3};

WaterMesh::WaterMesh()
{
    mSize = 1.0f;
    // mVertexDataMap = std::make_shared<std::unordered_map<std::string, std::vector<float>>>();
}

void WaterMesh::Init()
{
    // exit if parent does not have a transform component
    if (!mParent || !mParent->GetParent()->HasComponent<TransformComponent>())
    {
        std::cout << "Water Mesh component's parent does not have a transform component\n";
        exit(1);
    }

    glm::vec3 position = mParent->GetParent()->GetComponent<TransformComponent>()->GetPosition();
    mMinCorner = position - glm::vec3(mSize / 2.0);
    mMaxCorner = position + glm::vec3(mSize / 2.0);
}

WaterMesh::~WaterMesh()
{
    for (const auto side : mParent->GetVisibleSides())
    {
        OffloadFace(side);
    }
}

void WaterMesh::Update()
{
}

void WaterMesh::Render()
{
    if (!mInitialized)
    {
        SetVertexData();
    }

    for (const auto &face : mParent->GetVisibleSides())
    { 
        glBindVertexArray(mVAOs.at(face));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void WaterMesh::SetVertexData()
{
    if (mInitialized)
    {
        return;
    }

    // water data
    // x, y, z, r, g, b, nx, ny, nz
    float r = 0.0f, g = 0.5f, b = 1.0f;
    float radius = mSize / 2.0f;
    // initialize vertex data map
    std::vector<float> tTop{
        // Top
        -radius, radius, -radius, r, g, b, 0.f, 1.f, 0.f, // - + -
        -radius, radius, radius, r, g, b, 0.f, 1.f, 0.f,  // - + +
        radius, radius, radius, r, g, b, 0.f, 1.f, 0.f,   // + + +
        radius, radius, -radius, r, g, b, 0.f, 1.f, 0.f   // + + -
    };

    std::vector<float> tBottom{
        // Bottom
        -radius, -radius, radius, r, g, b, 0.f, -1.f, 0.f,  // - - +
        -radius, -radius, -radius, r, g, b, 0.f, -1.f, 0.f, // - - -
        radius, -radius, -radius, r, g, b, 0.f, -1.f, 0.f,  // + - -
        radius, -radius, radius, r, g, b, 0.f, -1.f, 0.f    // + - +
    };

    std::vector<float> tLeft{
        // Left
        -radius, radius, -radius, r, g, b, -1.f, 0.f, 0.f,  // - + -
        -radius, -radius, -radius, r, g, b, -1.f, 0.f, 0.f, // - - -
        -radius, -radius, radius, r, g, b, -1.f, 0.f, 0.f,  // - - +
        -radius, radius, radius, r, g, b, -1.f, 0.f, 0.f    // - + +
    };

    std::vector<float> tRight{
        // Right
        radius, radius, radius, r, g, b, 1.f, 0.f, 0.f,   // + + +
        radius, -radius, radius, r, g, b, 1.f, 0.f, 0.f,  // + - +
        radius, -radius, -radius, r, g, b, 1.f, 0.f, 0.f, // + - -
        radius, radius, -radius, r, g, b, 1.f, 0.f, 0.f   // + + -
    };

    std::vector<float> tFront{
        // Front
        -radius, radius, radius, r, g, b, 0.f, 0.f, 1.f,  // - + +
        -radius, -radius, radius, r, g, b, 0.f, 0.f, 1.f, // - - +
        radius, -radius, radius, r, g, b, 0.f, 0.f, 1.f,  // + - +
        radius, radius, radius, r, g, b, 0.f, 0.f, 1.f    // + + +
    };

    std::vector<float> tBack{
        // Back
        -radius, -radius, -radius, r, g, b, 0.f, 0.f, -1.f, // - - -
        -radius, radius, -radius, r, g, b, 0.f, 0.f, -1.f,  // - + -
        radius, radius, -radius, r, g, b, 0.f, 0.f, -1.f,   // + + -
        radius, -radius, -radius, r, g, b, 0.f, 0.f, -1.f   // + - -
    };

    mVertices.insert({"top", tTop});
    mVertices.insert({"bottom", tBottom});
    mVertices.insert({"left", tLeft});
    mVertices.insert({"right", tRight});
    mVertices.insert({"front", tFront});
    mVertices.insert({"back", tBack});

    std::vector<std::string> sides = {"top", "bottom", "left", "right", "front", "back"};

    for (const auto &side : sides)
    {
        LoadFace(side);
    }

    mInitialized = true;
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
    mVAOs.insert({face, vao});
    mVBOs.insert({face, vbo});
}

void WaterMesh::OffloadFace(std::string face)
{
    glDeleteVertexArrays(1, &mVAOs.at(face));
    glDeleteBuffers(1, &mVBOs.at(face));
    glDeleteBuffers(1, &mIBO);
    mParent->RemoveVisibleSide(face);
}