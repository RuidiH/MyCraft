#include "TextureManager.hpp"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::LoadTexture(const std::string &filePath)
{
    // skip loading if texture already exists
    if (GetTexture(filePath) != nullptr)
    {
        // std::cout << "Texture " + filePath + " already loaded" << std::endl;
        return;
    }

    // load the texture
    GLuint *textureId = new GLuint();
    // temp code of loading texture
    SDL_Surface *surface = IMG_Load(filePath.c_str());

    if (!surface)
    {
        std::cout << "ERROR LOADING IMAGE" << std::endl;
        // handle error
    }

    mTextureMap.insert(std::pair<std::string, GLuint *>(filePath, textureId));
    glGenTextures(1, textureId);
    glBindTexture(GL_TEXTURE_2D, *textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_FreeSurface(surface);

    std::cout << "Successfully loaded " << filePath << std::endl;
}

GLuint* TextureManager::GetTexture(const std::string &textureName)
{
    // find the textureName in the map, return the textureId if found
    if (mTextureMap.find(textureName) != mTextureMap.end())
    {
        return mTextureMap[textureName];
    }
    else
    {
        return nullptr;
    }
}

void TextureManager::LoadTextureGroup(const std::string &textureGroupName, std::array<std::string, 6> groupPaths)
{
    std::cout << "Loading texture group " + textureGroupName << std::endl;
    // avoid reloading textures
    if (mTextureGroupMap.find(textureGroupName) != mTextureGroupMap.end())
    {
        // std::cout << "Texture group " + textureGroupName + " already loaded" << std::endl;
        return;
    }

    std::array<GLuint *, 6> textureGroupArray;
    for (int i = 0; i < groupPaths.size(); i++)
    {
        GLuint* textureID = GetTexture(groupPaths[i]);
        if (textureID == nullptr)
        {
            LoadTexture(groupPaths[i]);
            textureID = GetTexture(groupPaths[i]);
        }
        textureGroupArray[i] = GetTexture(groupPaths[i]);
    }
    mTextureGroupMap.insert(std::pair<std::string, std::array<GLuint *, 6>>(textureGroupName, textureGroupArray));

    std::cout << "Successfully loaded texture group " + textureGroupName << std::endl;
}
