#include "WorldSerializer.hpp"
#include "TransformComponent.hpp"
#include "ShapeComponent.hpp"
#include "TextureComponent.hpp"
#include "Cube.hpp"

#include <fstream>
#include <array>

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

void WorldSerializer::SaveWorld(std::string filename, std::vector<std::shared_ptr<GameObject>> gameObjects, TextureManager &textureManager)
{
    std::map<std::string, std::string> textureMappings = textureManager.GetTexturePathMap();

    rapidjson::StringBuffer s;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

    writer.StartObject(); // start root
    writer.Key("world");
    writer.StartObject(); // start world
    // writer.StartArray();

    // SaveTexture(writer, textureMappings);

    // hard-coded object texture definitions
    writer.Key("textures");
    writer.StartArray();

    writer.StartObject(); // start snow
    writer.Key("name");
    writer.String("snow");
    writer.Key("values");
    writer.StartArray(); // start snow
    writer.String("./assets/texture/snow.png");
    writer.String("./assets/texture/dirt.png");
    writer.String("./assets/texture/grass_side_snowed.png");
    writer.String("./assets/texture/grass_side_snowed.png");
    writer.String("./assets/texture/grass_side_snowed.png");
    writer.String("./assets/texture/grass_side_snowed.png");
    writer.EndArray();  // end snow
    writer.EndObject(); // end snow

    writer.StartObject(); // start grass
    writer.Key("name");
    writer.String("grass");
    writer.Key("values");
    writer.StartArray(); // start grass
    writer.String("./assets/texture/grass_carried.png");
    writer.String("./assets/texture/dirt.png");
    writer.String("./assets/texture/grass_side_carried.png");
    writer.String("./assets/texture/grass_side_carried.png");
    writer.String("./assets/texture/grass_side_carried.png");
    writer.String("./assets/texture/grass_side_carried.png");
    writer.EndArray();  // end grass
    writer.EndObject(); // end grass

    writer.StartObject(); // start dirt
    writer.Key("name");
    writer.String("dirt");
    writer.Key("values");
    writer.StartArray(); // start dirt
    writer.String("./assets/texture/dirt.png");
    writer.String("./assets/texture/dirt.png");
    writer.String("./assets/texture/dirt.png");
    writer.String("./assets/texture/dirt.png");
    writer.String("./assets/texture/dirt.png");
    writer.String("./assets/texture/dirt.png");
    writer.EndArray(); // end dirt
    writer.EndObject();

    writer.EndArray();
    // writer.EndObject(); // end texture groups

    writer.Key("objects");
    writer.StartArray(); // start objects

    for (const auto &gameObject : gameObjects)
    {
        // save game object
        writer.StartObject();

        SaveObjectTransform(gameObject, writer);

        writer.Key("texture");
        writer.String(gameObject->GetComponent<TextureComponent>()->GetTextureGroupName().c_str());
        // SaveObjectShape(gameObject, writer, textureManager);

        writer.EndObject();
    }
    // End game objects
    writer.EndArray();  // End of the "objects" array
    writer.EndObject(); // End of the "world" object
    writer.EndObject(); // End of the root object

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
        // writer.StartObject();
        writer.Key("transform");
        writer.StartArray();

        writer.StartObject();
        writer.Key("position");
        writer.StartArray();
        writer.Double(transform->GetPosition().x);
        writer.Double(transform->GetPosition().y);
        writer.Double(transform->GetPosition().z);
        writer.EndArray();
        writer.EndObject();

        writer.StartObject();
        writer.Key("rotation");
        writer.StartArray();
        writer.Double(transform->GetRotation().x);
        writer.Double(transform->GetRotation().y);
        writer.Double(transform->GetRotation().z);
        writer.EndArray();
        writer.EndObject();

        writer.StartObject();
        writer.Key("scale");
        writer.StartArray();
        writer.Double(transform->GetScale().x);
        writer.Double(transform->GetScale().y);
        writer.Double(transform->GetScale().z);
        writer.EndArray();
        writer.EndObject();

        writer.EndArray();
        // writer.EndObject();
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

void WorldSerializer::LoadWorld(std::string filename, std::vector<std::shared_ptr<GameObject>> &gameObjects, TextureManager &textureManager)
{

    // Open the file
    FILE *fp = fopen(filename.c_str(), "rb"); // Use "r" for reading
    if (!fp)
    {
        std::cerr << "Cannot open file!" << std::endl;
        return;
    }

    char readBuffer[65536]; // Buffer for reading the file
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document d;
    d.ParseStream(is);
    fclose(fp); // Close the file as soon as it's read

    // Check if the structure is as expected
    if (!d.HasMember("world") || !d["world"].IsObject())
    {
        std::cerr << "JSON format is not as expected!" << std::endl;
        return;
    }

    const rapidjson::Value &world = d["world"];
    if (!world.HasMember("objects") || !world["objects"].IsArray())
    {
        std::cerr << "JSON format is not as expected!" << std::endl;
        return;
    }

    // handle textures
    const auto &textures = d["world"]["textures"].GetArray();

    for (const auto &texture : textures)
    {
        const auto &name = texture["name"].GetString();
        std::array<std::string, 6> texturePaths;
        std::cout << "Texture Name: " << name << std::endl;
        const auto &values = texture["values"].GetArray();
        for (int i = 0; i < values.Size(); i++)
        {
            std::cout << "Texture Path: " << values[i].GetString() << std::endl;
            textureManager.LoadTexture(values[i].GetString());
            texturePaths[i] = values[i].GetString();
        }
        textureManager.LoadTextureGroup(name, texturePaths);
    }

    // handle objects
    const auto &objects = d["world"]["objects"].GetArray();

    for (const auto &obj : objects)
    {
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
        gameObject->AddComponent<ShapeComponent>()->AddCube();
        gameObject->AddComponent<TransformComponent>();
        const auto &transformArray = obj["transform"].GetArray();
        for (const auto &transformComponent : transformArray)
        {
            if (transformComponent.HasMember("position"))
            {
                glm::vec3 position(transformComponent["position"][0].GetFloat(),
                                   transformComponent["position"][1].GetFloat(),
                                   transformComponent["position"][2].GetFloat());
                gameObject->GetComponent<TransformComponent>()->SetPosition(position);
            }
            if (transformComponent.HasMember("rotation"))
            {
                glm::vec3 rotation(transformComponent["rotation"][0].GetFloat(),
                                   transformComponent["rotation"][1].GetFloat(),
                                   transformComponent["rotation"][2].GetFloat());
                gameObject->GetComponent<TransformComponent>()->SetRotation(rotation);
            }
            if (transformComponent.HasMember("scale"))
            {
                glm::vec3 scale(transformComponent["scale"][0].GetFloat(),
                                transformComponent["scale"][1].GetFloat(),
                                transformComponent["scale"][2].GetFloat());
                gameObject->GetComponent<TransformComponent>()->SetScale(scale);
            }
        }

        if (obj.HasMember("texture"))
        {
            const auto &texture = obj["texture"].GetString();
            gameObject->AddComponent<TextureComponent>()->SetTextureGroupName(texture);
            gameObject->GetComponent<TextureComponent>()->SetTextureGroup(textureManager.GetTextureGroup(texture));
        }
        gameObjects.push_back(gameObject);
    }
}
