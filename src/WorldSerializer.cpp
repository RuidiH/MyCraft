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

WorldSerializer::WorldSerializer()
{
}

WorldSerializer::~WorldSerializer()
{
}

WorldSerializer::WorldSerializer(std::shared_ptr<TextureManager> textureManager, std::vector<std::shared_ptr<GameObject>> &objects) : mTextureManager(textureManager), mGameObjects(&objects)
{
    mGameObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>(objects);
}

void WorldSerializer::CreateBlocks(const std::string &filename)
{
    std::cout << "Creating blocks from " << filename << std::endl;
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    rapidjson::Document worldDocument;
    worldDocument.Parse(content.c_str());

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
        auto block = std::make_shared<GameObject>();

        // Assuming you have a method to set up components based on type
        for (const auto &comp : blockType.components)
        {
            // block->AddComponent(comp.first, comp.second); // Simplify, actual method might be different
            if (comp.first == "mesh")
            {
                if (comp.second.at("type") == "cube")
                {
                    block->AddComponent<MeshComponent>()->AddMesh(MeshType::CUBE);
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

        // Set the transform from the world data
        auto &transformData = blockData["transform"];
        glm::vec3 position(
            transformData["position"][0].GetFloat(),
            transformData["position"][1].GetFloat(),
            transformData["position"][2].GetFloat());
        glm::vec3 rotation(
            transformData["rotation"][0].GetFloat(),
            transformData["rotation"][1].GetFloat(),
            transformData["rotation"][2].GetFloat());
        glm::vec3 scale(
            transformData["scale"][0].GetFloat(),
            transformData["scale"][1].GetFloat(),
            transformData["scale"][2].GetFloat());

        block->AddComponent<TransformComponent>()->SetPosition(position);

        mGameObjects->push_back(block);
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

    std::unordered_map<std::string, BlockType> blockTypes;

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

        blockTypes[blockType.id] = blockType;
    }
    std::cout << "Block types read successfully!" << std::endl;
}

void WorldSerializer::LoadTextureConfig(const std::string &filename)
{
    std::cout << "Loading texture configuration from " << filename << std::endl;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    // Use std::stringstream to read the entire content
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close(); // Close the file as it's no longer needed

    // Convert the buffer into a string for RapidJSON parsing
    std::string fileContent = buffer.str();

    // Parse the JSON string using RapidJSON
    rapidjson::Document d;
    d.Parse(fileContent.c_str());

    // Extract texture paths into a map for easy lookup
    std::unordered_map<std::string, std::string> texturePaths;
    for (const auto &texture : d["path"].GetArray())
    {
        std::string name = texture["name"].GetString();
        std::string path = texture["path"].GetString();
        texturePaths[name] = path;
    }

    // Iterate over the texture groups and extract their details
    for (const auto &group : d["groups"].GetArray())
    {
        std::string groupName = group["name"].GetString();
        std::array<std::string, 6> paths;

        const auto &sides = group["sides"].GetArray();
        for (rapidjson::SizeType i = 0; i < sides.Size(); ++i)
        {
            paths[i] = texturePaths[sides[i].GetString()];
        }

        // Invoke the function to load each texture group with the retrieved paths
        mTextureManager->LoadTextureGroup(groupName, paths);
    }
    std::cout << "Texture configuration loaded successfully!" << std::endl;
}