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

// void TextureManager::LoadTexture(const std::string &filePath)
// {
//     // skip loading if texture already exists
//     if (GetTexture(filePath) != nullptr)
//     {
//         std::cout << "Texture " + filePath + " already loaded" << std::endl;
//         return;
//     }

//     // load the texture
//     GLuint *textureId = new GLuint();
//     // temp code of loading texture
//     SDL_Surface *surface = IMG_Load(filePath.c_str());

//     if (!surface)
//     {
//         std::cout << "ERROR LOADING IMAGE" << std::endl;
//         // handle error
//     }

//     mTextureMap.insert(std::pair<std::string, GLuint*>(filePath, textureId));
//     glGenTextures(1, textureId);
//     glBindTexture(GL_TEXTURE_2D, *textureId);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

//     SDL_FreeSurface(surface);

//     std::cout << "Successfully loaded " << filePath << std::endl; 
// }

GLuint* TextureManager::GetTexture(const std::string &textureName)
{
    // find the textureName in the map, return the textureId if found
    if (mTextureMap.find(textureName) != mTextureMap.end())
    {
        // std::cout << "Texture " + textureName + " found" << std::endl;
        return mTextureMap[textureName];
    }
    else
    {
        std::cout << "ERROR: Texture " << textureName << " not found" << std::endl;
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

void TextureManager::LoadTextureGroup(const std::string &textureGroupName, const std::map<std::string, std::string> &groupMap)
{
    std::cout << "Loading texture group " + textureGroupName << std::endl;   
    // avoid reloading textures
    if (mTextureGroupMap.find(textureGroupName) != mTextureGroupMap.end())
    {
        std::cout << "Texture group " + textureGroupName + " already loaded" << std::endl;
        return;
    }

    std::map<std::string, GLuint *> textureGroup;
    for (const auto &texture : groupMap)
    {
        textureGroup.insert(std::pair<std::string, GLuint *>(texture.first, GetTexture(texture.second)));
    }

    mTextureGroupMap.insert(std::pair<std::string, std::map<std::string, GLuint *>>(textureGroupName, textureGroup));
}
