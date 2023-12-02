#include "Cube.hpp"

#include <vector>
#include <iostream>
#include <SDL2/SDL_image.h>

Cube::Cube()
{
    mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    mSize = 1.0f;
}

Cube::Cube(glm::vec3 position, float size)
{
    mPosition = position;
    mSize = size;

    setVertexData();
}

// Cube::~Cube() {

// }

void Cube::Input()
{
}

void Cube::Update()
{
}

void Cube::Render()
{

    for (const auto &face : mVertexDataMap)
    {
        GLuint vao;

        GLuint vbo;

        GLuint ibo;

        GLuint *currentTexID = mTextureIdMap[face.first];

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *currentTexID);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     face.second.size() * sizeof(GL_FLOAT),
                     face.second.data(),
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

void Cube::setPosition(glm::vec3 position)
{
    mPosition = position;
}

void Cube::setVertexData()
{
    float radius = mSize / 2.0;

    // initialize vertex data map
    const std::vector<float> vTop{
        // Top
        mPosition.x - radius, mPosition.y + radius, mPosition.z - radius, 0.0f, 0.0f, 0.f, 1.f, 0.f, // - + -
        mPosition.x - radius, mPosition.y + radius, mPosition.z + radius, 0.0f, 1.0f, 0.f, 1.f, 0.f, // - + +
        mPosition.x + radius, mPosition.y + radius, mPosition.z + radius, 1.0f, 1.0f, 0.f, 1.f, 0.f, // + + +
        mPosition.x + radius, mPosition.y + radius, mPosition.z - radius, 1.0f, 0.0f, 0.f, 1.f, 0.f  // + + -
    };

    std::vector<float> vBottom{
        // Bottom
        mPosition.x - radius, mPosition.y - radius, mPosition.z + radius, 0.0f, 1.0f, 0.f, -1.f, 0.f, // - - +
        mPosition.x - radius, mPosition.y - radius, mPosition.z - radius, 0.0f, 0.0f, 0.f, -1.f, 0.f, // - - -
        mPosition.x + radius, mPosition.y - radius, mPosition.z - radius, 1.0f, 0.0f, 0.f, -1.f, 0.f, // + - -
        mPosition.x + radius, mPosition.y - radius, mPosition.z + radius, 1.0f, 1.0f, 0.f, -1.f, 0.f  // + - +
    };

    std::vector<float> vLeft{
        // Left
        mPosition.x - radius, mPosition.y + radius, mPosition.z - radius, 0.0f, 0.0f, -1.f, 0.f, 0.f, // - + -
        mPosition.x - radius, mPosition.y - radius, mPosition.z - radius, 0.0f, 1.0f, -1.f, 0.f, 0.f, // - - -
        mPosition.x - radius, mPosition.y - radius, mPosition.z + radius, 1.0f, 1.0f, -1.f, 0.f, 0.f, // - - +
        mPosition.x - radius, mPosition.y + radius, mPosition.z + radius, 1.0f, 0.0f, -1.f, 0.f, 0.f  // - + +
    };

    std::vector<float> vRight{
        // Right
        mPosition.x + radius, mPosition.y + radius, mPosition.z + radius, 0.0f, 0.0f, 1.f, 0.f, 0.f, // + + +
        mPosition.x + radius, mPosition.y - radius, mPosition.z + radius, 0.0f, 1.0f, 1.f, 0.f, 0.f, // + - +
        mPosition.x + radius, mPosition.y - radius, mPosition.z - radius, 1.0f, 1.0f, 1.f, 0.f, 0.f, // + - -
        mPosition.x + radius, mPosition.y + radius, mPosition.z - radius, 1.0f, 0.0f, 1.f, 0.f, 0.f  // + + -
    };

    std::vector<float> vFront{
        // Front
        mPosition.x - radius, mPosition.y + radius, mPosition.z + radius, 0.0f, 0.0f, 0.f, 0.f, 1.f, // - + +
        mPosition.x - radius, mPosition.y - radius, mPosition.z + radius, 0.0f, 1.0f, 0.f, 0.f, 1.f, // - - +
        mPosition.x + radius, mPosition.y - radius, mPosition.z + radius, 1.0f, 1.0f, 0.f, 0.f, 1.f, // + - +
        mPosition.x + radius, mPosition.y + radius, mPosition.z + radius, 1.0f, 0.0f, 0.f, 0.f, 1.f  // + + +
    };

    std::vector<float> vBack{
        // Back
        mPosition.x - radius, mPosition.y - radius, mPosition.z - radius, 1.0f, 1.0f, 0.f, 0.f, -1.f, // - - -
        mPosition.x - radius, mPosition.y + radius, mPosition.z - radius, 1.0f, 0.0f, 0.f, 0.f, -1.f, // - + -
        mPosition.x + radius, mPosition.y + radius, mPosition.z - radius, 0.0f, 0.0f, 0.f, 0.f, -1.f, // + + -
        mPosition.x + radius, mPosition.y - radius, mPosition.z - radius, 0.0f, 1.0f, 0.f, 0.f, -1.f  // + - -
    };

    mVertexDataMap["top"] = vTop;
    mVertexDataMap["bottom"] = vBottom;
    mVertexDataMap["left"] = vLeft;
    mVertexDataMap["right"] = vRight;
    mVertexDataMap["front"] = vFront;
    mVertexDataMap["back"] = vBack;
}
