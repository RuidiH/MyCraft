#ifndef WORLD_SERIALIZER_HPP
#define WORLD_SERIALIZER_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "GameObject.hpp"
#include "TextureManager.hpp"
#include "ObjectManager.hpp"

struct BlockType {
    std::string id;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> components;
    std::unordered_map<std::string, float> properties; 
};

class WorldSerializer
{
public:
    WorldSerializer(std::shared_ptr<TextureManager> textureManager, std::shared_ptr<ObjectManager> objectManager);
    ~WorldSerializer();

    void ReadBlockTypes(const std::string &filename);
    void LoadTextureConfig(const std::string &filename);
    void CreateBlocks(const std::string& filename);
    void SaveWorldData(const std::string& filename);
    // void SaveWorld(std::string filename, std::vector<std::shared_ptr<GameObject>> gameObjects, TextureManager &textureManager);
    // void LoadWorld(std::string filename, std::vector<std::shared_ptr<GameObject>> &gameObjects, TextureManager &textureManager);

private:
    std::unordered_map<std::string, BlockType> mblockTypes;
    std::shared_ptr<TextureManager> mTextureManager;
    std::shared_ptr<ObjectManager> mObjectManager;
};

#endif