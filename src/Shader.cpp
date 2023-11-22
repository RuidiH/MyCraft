#include "Shader.hpp"

#include <string>
#include <fstream>
#include <iostream>

Shader::Shader() {
}

void Shader::Init(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = LoadShaderAsString(vertexPath);
    std::string fragmentCode = LoadShaderAsString(fragmentPath);

    mProgramID = glCreateProgram();

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexCodeCStr = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vertexCodeCStr, NULL);
    glCompileShader(vertexShader);

    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
    const char* fragmentCodeCStr = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentCodeCStr, NULL);
    glCompileShader(fragmentShader);

    glAttachShader(mProgramID, vertexShader);
    glAttachShader(mProgramID, fragmentShader);
    glLinkProgram(mProgramID);
    glValidateProgram(mProgramID);
}

void Shader::Use() {
    glUseProgram(mProgramID);
}

void Shader::SetUniform(const char* name, GLuint value) const {
    GLuint uniform = glGetUniformLocation(mProgramID, name);
    if (uniform >= 0)
    {
        glUniform1i(uniform, value);
    }
    else
    {
        std::cout << "Could not find " << name <<" maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }
}

void Shader::SetUniform(const char* name, glm::vec3 value) const {
    GLuint uniform = glGetUniformLocation(mProgramID, name);
    if (uniform >= 0)
    {
        glUniform3f(uniform, value.x, value.y, value.z);
    }
    else
    {
        std::cout << "Could not find " << name <<" maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }
}

void Shader::SetUniform(const char* name, glm::mat4 matrix) const {
    GLuint uniform = glGetUniformLocation(mProgramID, name);
    if (uniform >= 0)
    {
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &matrix[0][0]);
    }
    else
    {
        std::cout << "Could not find " << name <<" maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }
}

std::string Shader::LoadShaderAsString(const std::string &filename)
{
    std::string result = "";

    std::string line = "";
    std::ifstream myFile(filename.c_str());

    if (myFile.is_open())
    {
        while (std::getline(myFile, line))
        {
            result += line + '\n';
        }
        myFile.close();
    }
    return result;
}