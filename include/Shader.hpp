#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader{
    public:
        Shader();

        void Init(const char* vertexPath, const char* fragmentPath);
        void Use();
        void SetUniform(const char* name, GLuint value) const;
        void SetUniform(const char* name, glm::vec3 value) const;
        void SetUniform(const char* name, glm::mat4 matrix) const; 

    private:
        GLuint mProgramID; 
        
        std::string LoadShaderAsString(const std::string &filename);
};

#endif