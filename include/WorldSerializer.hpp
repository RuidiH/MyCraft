#ifndef WORLD_SERIALIZER_HPP
#define WORLD_SERIALIZER_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "GameObject.hpp"
#include "TextureManager.hpp"

class WorldSerializer
{
public:
    WorldSerializer();
    ~WorldSerializer();

    void SaveWorld(std::string filename, std::vector<std::shared_ptr<GameObject>> gameObjects, TextureManager &textureManager);
    void LoadWorld(std::string filename, std::vector<std::shared_ptr<GameObject>> &gameObjects, TextureManager &textureManager);

private:
    void SaveTexture(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer, std::map<std::string, std::string> &textureMappings);
    // void SaveObjectShape(const std::shared_ptr<GameObject> &gameObject, rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer, TextureManager &textureManager);
    void SaveObjectTransform(const std::shared_ptr<GameObject> &gameObject, rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer);
};

#endif