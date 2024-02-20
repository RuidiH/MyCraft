#ifndef TEXTUREMANAGER_HPP 
#define TEXTUREMANAGER_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <SDL2/SDL_image.h>

#include <map>
#include <iostream>

class TextureManager {
    public:
        TextureManager();
        ~TextureManager();

        void LoadTexture(const std::string &textureName, const std::string &filePath);

        GLuint* GetTexture(const std::string &textureName);
        std::string FindTextureName(GLuint *textureId);

        const std::map<std::string, std::string> GetTexturePathMap() { return mTexturePathMap; }
    private:
        std::map<std::string, GLuint*> mTextureMap;
        std::map<std::string, std::string> mTexturePathMap;
};

#endif

