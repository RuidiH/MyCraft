#include "TextureManager.hpp"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    // TODO: verify copilot isn't lying to me
    for (auto &texture : mTextureMap)
    {
        glDeleteTextures(1, texture.second);
        delete texture.second;
    }
}

void TextureManager::LoadTexture(const std::string &textureName, const std::string &filePath)
{
    GLuint *textureId = new GLuint();
    // temp code of loading texture
    SDL_Surface *surface = IMG_Load(filePath.c_str());

    if (!surface)
    {
        std::cout << "ERROR LOADING IMAGE" << std::endl;
        // handle error
    }

    mTextureMap.insert(std::pair<std::string, GLuint*>(textureName, textureId));
    mTexturePathMap.insert(std::pair<std::string, std::string>(textureName, filePath));

    glGenTextures(1, textureId);
    glBindTexture(GL_TEXTURE_2D, *textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_FreeSurface(surface);

    std::cout << "Successfully loaded " << textureName << std::endl;
}

GLuint* TextureManager::GetTexture(const std::string &textureName)
{
    // find the textureName in the map, return the textureId if found
    auto texture = mTextureMap.find(textureName);
    if (texture != mTextureMap.end())
    {
        return texture->second;
    }
    else
    {
        std::cout << "ERROR: Texture not found" << std::endl;
        return nullptr;
    }
}

std::string TextureManager::FindTextureName(GLuint *textureId)
{
    for (auto &texture : mTextureMap)
    {
        if (texture.second == textureId)
        {
            return texture.first;
        }
    }
    return "";
}
