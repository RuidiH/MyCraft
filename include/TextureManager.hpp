#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <SDL2/SDL_image.h>

#include <map>
#include <iostream>
#include <vector>

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    // void LoadTexture(const std::string &textureName, const std::string &filePath);
    // void LoadTexture(const std::string &filePath);

    void LoadTexture(const std::string &filePath);
    GLuint *GetTexture(const std::string &textureName);

    void LoadTextureGroup(const std::string &textureGroupName, std::array<std::string, 6> groupPaths);

    std::array<GLuint *, 6> GetTextureGroup(const std::string &textureGroupName) { return mTextureGroupMap[textureGroupName]; }

private:
    std::map<std::string, GLuint *> mTextureMap;
    std::map<std::string, std::array<GLuint *, 6>> mTextureGroupMap;
};

#endif
