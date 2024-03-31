#include "WaterMesh.hpp"
#include "TransformComponent.hpp"

#include <iostream>

WaterMesh::WaterMesh()
{
    mSize = 1.0f;
    SetVertexData();
}

void WaterMesh::Update()
{
    // water cannot be moved!
}

void WaterMesh::Render()
{
    // render only the top surface of the water
    GLuint vao;
    GLuint vbo;
    GLuint ibo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 mVertexDataMap["top"].size() * sizeof(GL_FLOAT),
                 mVertexDataMap["top"].data(),
                 GL_STATIC_DRAW);

    // Set up Index Buffer Object
    std::vector<GLuint> indexBuffer{
        0, 1, 2,
        0, 2, 3};

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indexBuffer.size() * sizeof(GLuint),
                 indexBuffer.data(),
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
                          3, // normal
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 8,
                          (GLvoid *)(sizeof(GL_FLOAT) * 5));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
}

void WaterMesh::SetVertexData()
{
    float halfSize = mSize / 2.0f;

    // vertex data structure:
    // x, y, z, u, v, nx, ny, nz

    // initialize vertex data map
    std::vector<float> vTop{

        // Top
        -halfSize, halfSize, -halfSize, 0.0f, 0.0f, 0.f, 1.f, 0.f, // - + -
        -halfSize, halfSize, halfSize, 0.0f, 1.0f, 0.f, 1.f, 0.f,  // - + +
        halfSize, halfSize, halfSize, 1.0f, 1.0f, 0.f, 1.f, 0.f,   // + + +
        halfSize, halfSize, -halfSize, 1.0f, 0.0f, 0.f, 1.f, 0.f   // + + -
    };

    std::vector<float> vBottom{
        // Bottom
        -halfSize, -halfSize, halfSize, 0.0f, 1.0f, 0.f, -1.f, 0.f,  // - - +
        -halfSize, -halfSize, -halfSize, 0.0f, 0.0f, 0.f, -1.f, 0.f, // - - -
        halfSize, -halfSize, -halfSize, 1.0f, 0.0f, 0.f, -1.f, 0.f,  // + - -
        halfSize, -halfSize, halfSize, 1.0f, 1.0f, 0.f, -1.f, 0.f    // + - +
    };

    std::vector<float> vLeft{
        // Left
        -halfSize, halfSize, -halfSize, 0.0f, 0.0f, -1.f, 0.f, 0.f,  // - + -
        -halfSize, -halfSize, -halfSize, 0.0f, 1.0f, -1.f, 0.f, 0.f, // - - -
        -halfSize, -halfSize, halfSize, 1.0f, 1.0f, -1.f, 0.f, 0.f,  // - - +
        -halfSize, halfSize, halfSize, 1.0f, 0.0f, -1.f, 0.f, 0.f    // - + +
    };

    std::vector<float> vRight{
        // Right
        halfSize, halfSize, halfSize, 0.0f, 0.0f, 1.f, 0.f, 0.f,   // + + +
        halfSize, -halfSize, halfSize, 0.0f, 1.0f, 1.f, 0.f, 0.f,  // + - +
        halfSize, -halfSize, -halfSize, 1.0f, 1.0f, 1.f, 0.f, 0.f, // + - -
        halfSize, halfSize, -halfSize, 1.0f, 0.0f, 1.f, 0.f, 0.f   // + + -
    };

    std::vector<float> vFront{
        // Front
        -halfSize, halfSize, halfSize, 0.0f, 0.0f, 0.f, 0.f, 1.f,  // - + +
        -halfSize, -halfSize, halfSize, 0.0f, 1.0f, 0.f, 0.f, 1.f, // - - +
        halfSize, -halfSize, halfSize, 1.0f, 1.0f, 0.f, 0.f, 1.f,  // + - +
        halfSize, halfSize, halfSize, 1.0f, 0.0f, 0.f, 0.f, 1.f    // + + +
    };

    std::vector<float> vBack{
        // Back
        -halfSize, -halfSize, -halfSize, 1.0f, 1.0f, 0.f, 0.f, -1.f, // - - -
        -halfSize, halfSize, -halfSize, 1.0f, 0.0f, 0.f, 0.f, -1.f,  // - + -
        halfSize, halfSize, -halfSize, 0.0f, 0.0f, 0.f, 0.f, -1.f,   // + + -
        halfSize, -halfSize, -halfSize, 0.0f, 1.0f, 0.f, 0.f, -1.f   // + - -
    };

    mVertexDataMap["top"] = vTop;
    mVertexDataMap["bottom"] = vBottom;
    mVertexDataMap["left"] = vLeft;
    mVertexDataMap["right"] = vRight;
    mVertexDataMap["front"] = vFront;
    mVertexDataMap["back"] = vBack;
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