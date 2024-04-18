#include "WorldSerializer.hpp"
#include "TransformComponent.hpp"
#include "MeshComponent.hpp"
#include "TextureComponent.hpp"

#include <fstream>
#include <array>
#include <sstream>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

WorldSerializer::~WorldSerializer()
{
}

WorldSerializer::WorldSerializer(std::shared_ptr<TextureManager> textureManager, std::shared_ptr<ObjectManager> objManager) : mTextureManager(textureManager), mObjectManager(objManager)
{
}

void WorldSerializer::CreateBlocks(const std::string &filename)
{
    std::cout << "Creating blocks from " << filename << std::endl;
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    rapidjson::Document worldDocument;
    worldDocument.Parse(content.c_str());

    if (worldDocument.HasParseError() || !worldDocument.IsObject())
    {
        std::cerr << "JSON parse error or the document is not an object." << std::endl;
        return;
    } else {
        std::cout << "World document parsed successfully!" << std::endl;
    }

    for (const auto &blockData : worldDocument["world"]["blocks"].GetArray())
    {
        std::string blockId = blockData["id"].GetString();
        
        // Find the corresponding block type
        if (mblockTypes.find(blockId) == mblockTypes.end())
        {
            std::cerr << "Block type " << blockId << " not found!" << std::endl;
            continue;
        }

        const BlockType &blockType = mblockTypes.at(blockId);

        // Create and configure the block based on its type
        auto block = std::make_shared<GameObject>(blockId);

        // add transform
        auto &transformData = blockData["transform"];
        glm::vec3 position(
            transformData["position"][0].GetFloat(),
            transformData["position"][1].GetFloat(),
            transformData["position"][2].GetFloat());
        block->AddComponent<TransformComponent>()->SetPosition(position);

        // Assuming you have a method to set up components based on type
        for (const auto &comp : blockType.components)
        {
            // block->AddComponent(comp.first, comp.second); // Simplify, actual method might be different
            if (comp.first == "mesh")
            {
                if (comp.second.at("type") == "cube")
                { 
                    block->AddComponent<MeshComponent>(MeshType::CUBE);
                } else if (comp.second.at("type") == "water") {
                    block->AddComponent<MeshComponent>(MeshType::WATER);
                }
            }
            else if (comp.first == "texture")
            {
                if (comp.second.find("group") != comp.second.end())
                {
                    block->AddComponent<TextureComponent>()
                        ->SetTextureGroupName(comp.second.at("group"));

                    block->GetComponent<TextureComponent>()
                        ->SetTextureGroup(mTextureManager
                                              ->GetTextureGroup(comp.second.at("group")));
                }
            }
        }

        mObjectManager->AddObject(block);
        // mGameObjects->push_back(block);
    }
    std::cout << "Blocks created successfully!" << std::endl;
}

void WorldSerializer::ReadBlockTypes(const std::string &filename)
{
    std::cout << "Reading block types from " << filename << std::endl;
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    rapidjson::Document d;
    d.Parse(content.c_str());

    if (d.HasParseError() || !d.IsObject())
    {
        std::cerr << "JSON parse error or the document is not an object." << std::endl;
        return;
    }

    if (!d.HasMember("blockTypes") || !d["blockTypes"].IsArray())
    {
        std::cerr << "'blockTypes' not found or is not an array." << std::endl;
        return;
    }

    // std::unordered_map<std::string, BlockType> blockTypes;

    const auto &types = d["blockTypes"].GetArray();
    for (const auto &type : types)
    {
        BlockType blockType;
        blockType.id = type["id"].GetString();

        if (type.HasMember("properties"))
        {
            for (auto &m : type["properties"].GetObject())
            {
                blockType.properties[m.name.GetString()] = m.value.GetFloat();
            }
        }

        if (type.HasMember("components") && type["components"].IsObject())
        {
            const auto &components = type["components"].GetObject();

            for (const auto &comp : components)
            {
                const std::string componentName = comp.name.GetString(); // e.g., "mesh" or "texture"
                std::unordered_map<std::string, std::string> componentDetails;

                // Assuming the component itself can have multiple properties, like "type" or "group"
                if (comp.value.IsObject())
                {
                    for (const auto &detail : comp.value.GetObject())
                    {
                        // Here, 'detail.name.GetString()' could be "type", "group", etc.,
                        // and 'detail.value.GetString()' gives the corresponding value
                        componentDetails[detail.name.GetString()] = detail.value.GetString();
                    }
                }

                // Store the details map in the outer components map
                blockType.components[componentName] = componentDetails;
            }
        }

        mblockTypes[blockType.id] = blockType;
    }

    std::cout << "Block types read successfully!" << std::endl;
}

void WorldSerializer::LoadTextureConfig(const std::string &filename)
{
    std::cout << "Loading texture configuration from " << filename << std::endl;

    // Open and read the file content
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string fileContent = buffer.str();

    rapidjson::Document d;
    d.Parse(fileContent.c_str());

    // Check if parsing succeeded
    if (d.HasParseError())
    {
        std::cerr << "JSON parsing error." << std::endl;
        return;
    }

    // Ensure the document is an object and has the expected members
    if (!d.IsObject() || !d.HasMember("path") || !d["path"].IsArray() ||
        !d.HasMember("groups") || !d["groups"].IsArray())
    {
        std::cerr << "Invalid JSON format." << std::endl;
        return;
    }

    // Extract texture paths into a map for easy lookup
    std::unordered_map<std::string, std::string> texturePaths;
    for (const auto &texture : d["path"].GetArray())
    {
        // Notice the trimming to handle potential whitespace issues in "name"
        std::string name = texture["name"].GetString();
        name.erase(name.find_last_not_of(" \n\r\t") + 1); // Trim trailing spaces if any
        std::string path = texture["path"].GetString();
        texturePaths[name] = path;
    }

    // Iterate over the texture groups and extract their details
    for (const auto &group : d["groups"].GetArray())
    {
        if (!group.HasMember("name") || !group["name"].IsString() ||
            !group.HasMember("sides") || !group["sides"].IsArray())
        {
            continue; // Skip malformed entries
        }
        std::string groupName = group["name"].GetString();
        std::array<std::string, 6> paths;

        const auto &sides = group["sides"].GetArray();
        if (sides.Size() != 6)
        {
            std::cerr << "Invalid number of sides for group: " << groupName << std::endl;
            continue; // Ensure there are exactly 6 sides
        }
        for (rapidjson::SizeType i = 0; i < sides.Size(); ++i)
        {
            std::string sideName = sides[i].GetString();
            if (texturePaths.find(sideName) == texturePaths.end())
            {
                std::cerr << "Texture name not found: " << sideName << std::endl;
                break; // Skip this group if any texture name is not found
            }
            paths[i] = texturePaths[sideName];
        }

        // Invoke the function to load each texture group with the retrieved paths
        // Assuming mTextureManager is accessible and LoadTextureGroup is correctly implemented
        mTextureManager->LoadTextureGroup(groupName, paths);
    }
    std::cout << "Texture configuration loaded successfully!" << std::endl;
}

void WorldSerializer::SaveWorldData(const std::string& filename) {
    rapidjson::Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    rapidjson::Value worldObj(rapidjson::kObjectType);
    rapidjson::Value blocksArray(rapidjson::kArrayType);

    for (const auto& pair : mObjectManager->GetObjects()) {
        const auto& gameObject = pair.second;

        rapidjson::Value blockObj(rapidjson::kObjectType);
        // Assuming `GetID()` method exists
        blockObj.AddMember("id", rapidjson::Value(gameObject->GetID().c_str(), allocator), allocator);

        // Assuming `GetTransformComponent()` and it returns a valid TransformComponent
        auto transformComponent = gameObject->GetComponent<TransformComponent>();
        if (transformComponent) {
            rapidjson::Value transformObj(rapidjson::kObjectType);
            rapidjson::Value positionArray(rapidjson::kArrayType);

            auto position = transformComponent->GetPosition();
            positionArray.PushBack(rapidjson::Value(position.x), allocator);
            positionArray.PushBack(rapidjson::Value(position.y), allocator);
            positionArray.PushBack(rapidjson::Value(position.z), allocator);

            transformObj.AddMember("position", positionArray, allocator);
            blockObj.AddMember("transform", transformObj, allocator);
        }

        blocksArray.PushBack(blockObj, allocator);
    }

    worldObj.AddMember("blocks", blocksArray, allocator);
    d.AddMember("world", worldObj, allocator);

    // Serialize to string
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    // Write to file
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    ofs << buffer.GetString();
    ofs.close();

    std::cout << "World data saved to " << filename << std::endl;
}