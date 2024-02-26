#ifndef TEXTURE_COMPONENT_HPP
#define TEXTURE_COMPONENT_HPP

#include <glad/glad.h>

#include <string>
#include <map>
#include <iostream>
#include <array>

#include "Component.hpp"

class TextureComponent : public Component
{
public:
    TextureComponent() {};
    ~TextureComponent() {};

    void Input() {};
    void Update() {};
    void Render() {};

    void SetTextureGroupName(std::string texture) { mTexutureGroupName = texture; }
    void SetTextureGroup(std::array<GLuint *, 6> textureGroup); 
    std::string GetTextureGroupName() {  return mTexutureGroupName; }; 
    std::map<std::string, GLuint *> GetTextureGroup() { return mTextureGroup; }

private:
    std::string mTexutureGroupName;
    std::map<std::string, GLuint *> mTextureGroup;
};

#endif