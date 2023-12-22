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

    private:
        std::map<std::string, GLuint*> mTextureMap;

};

#endif

