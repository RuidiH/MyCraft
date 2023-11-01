#include "Cube.hpp"

#include <vector>
#include <iostream>
#include <SDL2/SDL_image.h>

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

    // std::cout << "Rendering Cube" << std::endl;
    // std::cout << mPosition.x << ", " << mPosition.y << ", " << mPosition.z << std::endl;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint vao;

    GLuint vbo;

    GLuint ibo;

    float radius = mSize/2.0;

    // const std::vector<float> vertexData{
    //     /*                      coordinates                            */   /*          colors         */               
    //     mPosition.x - radius, mPosition.y + radius, mPosition.z - radius,   mColor.x, mColor.y, mColor.z,     // - + -; 0

    //     mPosition.x + radius, mPosition.y + radius, mPosition.z - radius,   mColor.x, mColor.y, mColor.z,    // + + -; 1

    //     mPosition.x - radius, mPosition.y + radius, mPosition.z + radius,   mColor.x, mColor.y, mColor.z, // - + +; 2

    //     mPosition.x + radius, mPosition.y + radius, mPosition.z + radius,   mColor.x, mColor.y, mColor.z, // + + +; 3

    //     mPosition.x - radius, mPosition.y - radius, mPosition.z - radius,   mColor.x, mColor.y, mColor.z,  // - - -; 4

    //     mPosition.x + radius, mPosition.y - radius, mPosition.z - radius,   mColor.x, mColor.y, mColor.z,    // + - -; 5

    //     mPosition.x - radius, mPosition.y - radius, mPosition.z + radius,   mColor.x, mColor.y, mColor.z, // - - +; 6

    //     mPosition.x + radius, mPosition.y - radius, mPosition.z + radius,   mColor.x, mColor.y, mColor.z // + - +; 7
    // };

    const std::vector<float> vertexData{
        // Top
        mPosition.x - radius, mPosition.y + radius, mPosition.z - radius,   0.0f, 0.0f, // - + -
        mPosition.x - radius, mPosition.y + radius, mPosition.z + radius,   0.0f, 1.0f, // - + +
        mPosition.x + radius, mPosition.y + radius, mPosition.z + radius,   1.0f, 1.0f, // + + +
        mPosition.x + radius, mPosition.y + radius, mPosition.z - radius,   1.0f, 0.0f, // + + -

        // Left
        mPosition.x - radius, mPosition.y + radius, mPosition.z + radius,   0.0f, 0.0f, // - + +
        mPosition.x - radius, mPosition.y - radius, mPosition.z + radius,   1.0f, 0.0f, // - - +
        mPosition.x - radius, mPosition.y - radius, mPosition.z - radius,   1.0f, 1.0f, // - - -
        mPosition.x - radius, mPosition.y + radius, mPosition.z - radius,   0.0f, 1.0f, // - + -

        // Right
        mPosition.x + radius, mPosition.y + radius, mPosition.z + radius,   1.0f, 1.0f, // + + +
        mPosition.x + radius, mPosition.y - radius, mPosition.z + radius,   0.0f, 1.0f, // + - +
        mPosition.x + radius, mPosition.y - radius, mPosition.z - radius,   0.0f, 0.0f, // + - -
        mPosition.x + radius, mPosition.y + radius, mPosition.z - radius,   1.0f, 0.0f, // + + -

        // Front
        mPosition.x + radius, mPosition.y + radius, mPosition.z + radius,   1.0f, 1.0f, // + + +
        mPosition.x + radius, mPosition.y - radius, mPosition.z + radius,   1.0f, 0.0f, // + - +
        mPosition.x - radius, mPosition.y - radius, mPosition.z + radius,   0.0f, 0.0f, // - - +
        mPosition.x - radius, mPosition.y + radius, mPosition.z + radius,   0.0f, 1.0f, // - + +

        // Back
        mPosition.x + radius, mPosition.y + radius, mPosition.z - radius,   0.0f, 0.0f, // + + -
        mPosition.x + radius, mPosition.y - radius, mPosition.z - radius,   0.0f, 1.0f, // + - -
        mPosition.x - radius, mPosition.y - radius, mPosition.z - radius,   1.0f, 1.0f, // - - -
        mPosition.x - radius, mPosition.y + radius, mPosition.z - radius,   1.0f, 0.0f, // - + -

        // Bottom
        mPosition.x - radius, mPosition.y - radius, mPosition.z - radius,   1.0f, 1.0f, // - - -
        mPosition.x - radius, mPosition.y - radius, mPosition.z + radius,   1.0f, 0.0f, // - - +
        mPosition.x + radius, mPosition.y - radius, mPosition.z + radius,   0.0f, 0.0f, // + - +
        mPosition.x + radius, mPosition.y - radius, mPosition.z - radius,   0.0f, 1.0f, // + - -
    };

    glBindTexture(GL_TEXTURE_2D, *mTexId);    
    glActiveTexture(GL_TEXTURE0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(GL_FLOAT),
                 vertexData.data(),
                 GL_STATIC_DRAW);

    // const std::vector<GLuint> indexBufferData {
    //     0, 1, 2,    1, 3, 2,    // top
    //     2, 0, 6,    0, 4, 6,    // left
    //     3, 1, 7,    1, 5, 7,    // right
    //     4, 5, 6,    5, 7, 6,    // bottom
    //     0, 1, 4,    1, 5, 4,    // front
    //     2, 3, 6,    3, 7, 6     // back
    // };

    const std::vector<GLuint> indexBufferData{
        // Top
        0, 1, 2,
        0, 2, 3, 

        // Left
        5, 4, 6, 
        6, 4, 7, 

        // Right
        8, 9, 10, 
        8, 10, 11,    

        // Front
        13, 12, 14, 
        15, 14, 12,

        // Back
        16, 17, 18,
        16, 18, 19,  

        // Bottom
        21, 20, 22, 
        22, 20, 23
    };


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
                          sizeof(GL_FLOAT) * 5,
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2, // u, v
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT) * 5,
                          (GLvoid *)(sizeof(GL_FLOAT) * 3));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // std::cout << "End of rendering cube" << std::endl;
}

void Cube::setPosition(glm::vec3 position) {
    mPosition = position;
}

void Cube::setColor(glm::vec3 color) {
    mColor = color;
}

// bool Cube::operator==(const Cube &other) const {
//     return (mPosition == other.mPosition &&
//             mColor == other.mColor &&
//             mSize == other.mSize);
// }