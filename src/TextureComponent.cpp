#include "TextureComponent.hpp"

// void TextureComponent::SetTextureGroupName(std::string texture)
// {
//     mTexuture = texture;     
// }

void TextureComponent::SetTextureGroup(std::array<GLuint *, 6> textureGroup)
{
    mTextureGroup["top"] = textureGroup[0];
    mTextureGroup["bottom"] = textureGroup[1];
    mTextureGroup["left"] = textureGroup[2];
    mTextureGroup["right"] = textureGroup[3];
    mTextureGroup["front"] = textureGroup[4];
    mTextureGroup["back"] = textureGroup[5];
}