#include "Cube.hpp"

#include <vector>

Cube::Cube() {
    mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    mColor = glm::vec3(1.0f, 0.5f, 0.2f);
    mSize = 1.0f; 
}

// Cube::~Cube() {

// }

void Cube::Input() {

}

void Cube::Update() {

}

void Cube::Render() {

    GLuint vao;

    GLuint vbo;

    GLuint ibo;

    // initialize cube vertices
    const std::vector<float> vertexData{
        mPosition.x, mPosition.y, mPosition.z, // 0
        mColor.x, mColor.y, mColor.z, // mColor

        mPosition.x + mSize, mPosition.y, mPosition.z, // 1
        mColor.x, mColor.y, mColor.z, // mColor

        mPosition.x + mSize, mPosition.y + mSize, mPosition.z, // 2
        mColor.x, mColor.y, mColor.z, // mColor

        mPosition.x + mSize, mPosition.y + mSize, mPosition.z + mSize, // 3
        mColor.x, mColor.y, mColor.z, // mColor

        mPosition.x, mPosition.y + mSize, mPosition.z, // 4
        mColor.x, mColor.y, mColor.z, // mColor

        mPosition.x, mPosition.y, mPosition.z + mSize, // 5
        mColor.x, mColor.y, mColor.z, // mColor

        mPosition.x, mPosition.y + mSize, mPosition.z + mSize, // 6
        mColor.x, mColor.y, mColor.z, // mColor

        mPosition.x + mSize, mPosition.y, mPosition.z + mSize, // 7
        mColor.x, mColor.y, mColor.z, // mColor
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(GL_FLOAT),
                 vertexData.data(),
                 GL_STATIC_DRAW);

    const std::vector<GLuint> indexBufferData{0, 4, 2, 1, 0, 2,
                                              7, 3, 2, 1, 7, 2,
                                              0, 5, 7, 1, 0, 7,
                                              5, 6, 3, 7, 5, 3,
                                              4, 6, 3, 2, 4, 3,
                                              5, 6, 4, 0, 5, 4};

    // Set up Index Buffer Object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,   
                 indexBufferData.size() * sizeof(GLuint),
                 indexBufferData.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3, // x, y, z
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 6,
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3, // r, g, b
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 6,
                          (GLvoid *)(sizeof(GL_FLOAT) * 3));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

// bool Cube::operator==(const Cube &other) const {
//     return (mPosition == other.mPosition &&
//             mColor == other.mColor &&
//             mSize == other.mSize);
// }