#include "CubeMesh.hpp"
#include "TransformComponent.hpp"

CubeMesh::CubeMesh()
{
    mSize = 1.0f;
    SetVertexData();
}


void CubeMesh::Update()
{
    glm::vec3 position = mParent->GetParent()->GetComponent<TransformComponent>()->GetPosition();
    mMinCorner = position - glm::vec3(mSize / 2.0);
    mMaxCorner = position + glm::vec3(mSize / 2.0);
}

void CubeMesh::Render()
{
}

void CubeMesh::SetVertexData()
{
    float radius = mSize / 2.0;

    // vertex data structure:
    // x, y, z, u, v, nx, ny, nz

    // initialize vertex data map
    std::vector<float> vTop{

        // Top
        -radius, radius, -radius, 0.0f, 0.0f, 0.f, 1.f, 0.f, // - + -
        -radius, radius, radius, 0.0f, 1.0f, 0.f, 1.f, 0.f,  // - + +
        radius, radius, radius, 1.0f, 1.0f, 0.f, 1.f, 0.f,   // + + +
        radius, radius, -radius, 1.0f, 0.0f, 0.f, 1.f, 0.f   // + + -
    };

    std::vector<float> vBottom{
        // Bottom
        -radius, -radius, radius, 0.0f, 1.0f, 0.f, -1.f, 0.f,  // - - +
        -radius, -radius, -radius, 0.0f, 0.0f, 0.f, -1.f, 0.f, // - - -
        radius, -radius, -radius, 1.0f, 0.0f, 0.f, -1.f, 0.f,  // + - -
        radius, -radius, radius, 1.0f, 1.0f, 0.f, -1.f, 0.f    // + - +
    };

    std::vector<float> vLeft{
        // Left
        -radius, radius, -radius, 0.0f, 0.0f, -1.f, 0.f, 0.f,  // - + -
        -radius, -radius, -radius, 0.0f, 1.0f, -1.f, 0.f, 0.f, // - - -
        -radius, -radius, radius, 1.0f, 1.0f, -1.f, 0.f, 0.f,  // - - +
        -radius, radius, radius, 1.0f, 0.0f, -1.f, 0.f, 0.f    // - + +
    };

    std::vector<float> vRight{
        // Right
        radius, radius, radius, 0.0f, 0.0f, 1.f, 0.f, 0.f,   // + + +
        radius, -radius, radius, 0.0f, 1.0f, 1.f, 0.f, 0.f,  // + - +
        radius, -radius, -radius, 1.0f, 1.0f, 1.f, 0.f, 0.f, // + - -
        radius, radius, -radius, 1.0f, 0.0f, 1.f, 0.f, 0.f   // + + -
    };

    std::vector<float> vFront{
        // Front
        -radius, radius, radius, 0.0f, 0.0f, 0.f, 0.f, 1.f,  // - + +
        -radius, -radius, radius, 0.0f, 1.0f, 0.f, 0.f, 1.f, // - - +
        radius, -radius, radius, 1.0f, 1.0f, 0.f, 0.f, 1.f,  // + - +
        radius, radius, radius, 1.0f, 0.0f, 0.f, 0.f, 1.f    // + + +
    };

    std::vector<float> vBack{
        // Back
        -radius, -radius, -radius, 1.0f, 1.0f, 0.f, 0.f, -1.f, // - - -
        -radius, radius, -radius, 1.0f, 0.0f, 0.f, 0.f, -1.f,  // - + -
        radius, radius, -radius, 0.0f, 0.0f, 0.f, 0.f, -1.f,   // + + -
        radius, -radius, -radius, 0.0f, 1.0f, 0.f, 0.f, -1.f   // + - -
    };

    mVertexDataMap["top"] = vTop;
    mVertexDataMap["bottom"] = vBottom;
    mVertexDataMap["left"] = vLeft;
    mVertexDataMap["right"] = vRight;
    mVertexDataMap["front"] = vFront;
    mVertexDataMap["back"] = vBack;
}

glm::vec3 CubeMesh::GetSideNormal(std::string side) {
    if (side == "top") {
        return glm::vec3(0.f, 1.f, 0.f);
    } else if (side == "bottom") {
        return glm::vec3(0.f, -1.f, 0.f);
    } else if (side == "left") {
        return glm::vec3(-1.f, 0.f, 0.f);
    } else if (side == "right") {
        return glm::vec3(1.f, 0.f, 0.f);
    } else if (side == "front") {
        return glm::vec3(0.f, 0.f, 1.f);
    } else if (side == "back") {
        return glm::vec3(0.f, 0.f, -1.f);
    }
    std::cout << "Invalid side\n";
    return glm::vec3(0.f, 0.f, 0.f);
}