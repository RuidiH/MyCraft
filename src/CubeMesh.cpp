#include "CubeMesh.hpp"
#include "TransformComponent.hpp"
#include "TextureComponent.hpp"

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
    std::map<std::string, GLuint *> textureIdMap = mParent->GetParent()->GetComponent<TextureComponent>()->GetTextureGroup();
    for (const auto &face : textureIdMap)
    {
        GLuint vao;

        GLuint vbo;

        GLuint ibo;

        GLuint *currentTexID = face.second;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *currentTexID);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     mVertexDataMap[face.first].size() * sizeof(GL_FLOAT),
                     mVertexDataMap[face.first].data(),
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