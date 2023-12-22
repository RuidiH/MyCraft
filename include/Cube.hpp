#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <vector>

// #include "ShapeComponent.hpp"

class Cube {
    public:
        Cube();
        Cube(glm::vec3 position, float size);
        // ~Cube();
        void Input();
        void Update();
        void Render(); 

        /* Getter and setter functions */
        void setPosition(glm::vec3 position); // call this function before setVertexData()
        void setVertexData();

        void setTexture(GLuint *texId) {
            mTexId = texId;
        }

        void setFaceTexture(std::string face, GLuint *texId) {
            mTextureIdMap[face] = texId;
        }

    private:
        std::map<std::string, std::vector<float>> mVertexDataMap;
        std::map<std::string, std::vector<GLuint>> mIndexBufferMap;
        std::map<std::string, GLuint*> mTextureIdMap;
        glm::vec3 mPosition;
        float mSize;
        GLuint *mTexId;
};

#endif // CUBE_HPP