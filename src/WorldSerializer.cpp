#include "WorldSerializer.hpp"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "TransformComponent.hpp"
#include "ShapeComponent.hpp"
#include "Cube.hpp"

#include <fstream>

WorldSerializer::WorldSerializer()
{
}

WorldSerializer::~WorldSerializer()
{
}

void WorldSerializer::SaveWorld(std::string filename, std::vector<std::shared_ptr<GameObject>> gameObjects, TextureManager& textureManager)
{
    std::map<std::string, std::string> textureMappings = textureManager.GetTexturePathMap();

    rapidjson::StringBuffer s;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("World");
    writer.StartArray();

    SaveTexture(writer, textureMappings);

    writer.StartObject();
    writer.Key("GameObjects");
    writer.StartArray();

    for (const auto &gameObject : gameObjects)
    {
        // save game object
        writer.StartObject();
        writer.Key("Object");
        writer.StartArray();
        
        SaveObjectTransform(gameObject, writer);

        SaveObjectShape(gameObject, writer, textureManager);

        writer.EndArray();
        writer.EndObject();
    }
    // End game objects
    writer.EndArray();
    writer.EndObject();

    // End world
    writer.EndArray();
    writer.EndObject();

    // write to file
    std::ofstream file;
    file.open(filename);
    file << s.GetString();
    file.close();
}

void WorldSerializer::SaveObjectShape(const std::shared_ptr<GameObject> &gameObject, rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer, TextureManager &textureManager)
{
    ShapeComponent *shape = gameObject->GetComponent<ShapeComponent>();
    if (shape != nullptr)
    {
        Cube *cube = shape->GetCubes()[0].get();
        std::map<std::string, GLuint *> textureIdMap = cube->GetTextureIdMap();
        writer.StartObject();
        writer.Key("ShapeComponent");
        writer.StartArray();

        std::vector<std::string> faceNames = {"top", "bottom", "front", "back", "left", "right"};

        for (const auto &face : faceNames)
        {
            writer.StartObject();
            writer.Key(face.c_str());
            writer.String(textureManager.FindTextureName(textureIdMap[face]).c_str());
            writer.EndObject();
        }

        writer.EndArray();
        writer.EndObject();
    }
}

void WorldSerializer::SaveObjectTransform(const std::shared_ptr<GameObject> &gameObject, rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer)
{
    TransformComponent *transform = gameObject->GetComponent<TransformComponent>();
    if (transform != nullptr)
    {
        writer.StartObject();
        writer.Key("TransformComponent");
        writer.StartArray();
        writer.StartObject();
        writer.Key("Position");
        writer.StartArray();
        writer.Double(transform->GetPosition().x);
        writer.Double(transform->GetPosition().y);
        writer.Double(transform->GetPosition().z);
        writer.EndArray();
        writer.EndObject();

        writer.StartObject();
        writer.Key("Rotation");
        writer.StartArray();
        writer.Double(transform->GetRotation().x);
        writer.Double(transform->GetRotation().y);
        writer.Double(transform->GetRotation().z);
        writer.EndArray();
        writer.EndObject();
        writer.StartObject();
        writer.Key("Scale");
        writer.StartArray();
        writer.Double(transform->GetScale().x);
        writer.Double(transform->GetScale().y);
        writer.Double(transform->GetScale().z);
        writer.EndArray();
        writer.EndObject();
        writer.EndArray();
        writer.EndObject();
    }
}

void WorldSerializer::SaveTexture(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer, std::map<std::string, std::string> &textureMappings)
{
    // save textures and paths
    writer.StartObject();
    writer.Key("Textures");
    writer.StartArray();

    for (const auto &pair : textureMappings)
    {
        writer.StartObject();
        writer.Key(pair.first.c_str());
        writer.String(pair.second.c_str());
        writer.EndObject();
    }

    writer.EndArray();
    writer.EndObject();
}

void WorldSerializer::LoadWorld(std::string filename)
{
}
