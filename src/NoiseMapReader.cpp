#include "NoiseMapReader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

#include "TransformComponent.hpp"
#include "MeshComponent.hpp"
#include "TextureComponent.hpp"
#include "Component.hpp"

// Constructor
NoiseMapReader::NoiseMapReader(const std::string &filename) : filename(filename) {}

// Method to read the noise map
void NoiseMapReader::readNoiseMap()
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<double> row;
        std::istringstream sstream(line);
        std::string field;
        while (getline(sstream, field, ','))
        {
            row.push_back(std::stod(field));
        }
        noiseMap.push_back(row);
    }

    file.close();
}

// Getter for the noise map
const std::vector<std::vector<double>> &NoiseMapReader::getNoiseMap() const
{
    return noiseMap;
}

void NoiseMapReader::GenerateWorld(std::shared_ptr<ObjectManager> objectManager, const std::shared_ptr<TextureManager> &textureManager)
{

    int x_start = -1 * noiseMap.size() / 2;
    int z_start = -1 * noiseMap[0].size() / 2;
    int y_start = -5;

    // generate base dirt layer
    for (int i = 0; i < noiseMap.size(); i++)
    {
        for (int j = 0; j < noiseMap[i].size(); j++)
        {

            int xPosition = x_start + i;
            int zPosition = z_start + j;

            int yPosition = noiseMap[i][j] * 10 / 2;

            // dirt
            for (int k = -2; k < yPosition - 2; k++)
            {
                // grass
                if (k == yPosition - 3 && k > -1)
                {
                    // auto grassBlock = std::make_shared<GameObject>("grass_block");
                    // grassBlock->AddComponent<TransformComponent>(glm::vec3(xPosition, k, zPosition));
                    // grassBlock->AddComponent<MeshComponent>(MeshType::CUBE);
                    // grassBlock->AddComponent<TextureComponent>()->SetTextureGroupName("grass");
                    // grassBlock->GetComponent<TextureComponent>()->SetTextureGroup(textureManager->GetTextureGroup("grass"));
                    // objectManager->AddObject(grassBlock);
                }
                else
                {
                    auto dirtBlock = std::make_shared<GameObject>("dirt_block");
                    dirtBlock->AddComponent<TransformComponent>(glm::vec3(xPosition, k, zPosition));
                    dirtBlock->AddComponent<MeshComponent>(MeshType::CUBE);
                    dirtBlock->AddComponent<TextureComponent>()->SetTextureGroupName("dirt");
                    dirtBlock->GetComponent<TextureComponent>()->SetTextureGroup(textureManager->GetTextureGroup("dirt"));
                    objectManager->AddObject(dirtBlock);
                }
            }

            // water
            // for (int k = yPosition - 2; k < 0; k++)
            // {
            //     auto cube = std::make_shared<GameObject>("water_block");
            //     cube->AddComponent<TransformComponent>(glm::vec3(xPosition, k, zPosition));
            //     cube->AddComponent<MeshComponent>(MeshType::WATER);
            //     objectManager->AddObject(cube);
            // }
        }
    }

    // count number of object we are rendering
    std::cout << "Number of objects: " << objectManager->GetObjects().size() << std::endl;

    // count number of grass blocks
    int grassCount = 0;
    for (auto object : objectManager->GetObjects())
    {
        if (object.second->GetID() == "grass_block")
        {
            grassCount++;
        }
    }
    std::cout << "Number of grass blocks: " << grassCount << std::endl;

    // count number of dirt blocks
    int dirtCount = 0;
    for (auto object : objectManager->GetObjects())
    {
        if (object.second->GetID() == "dirt_block")
        {
            dirtCount++;
        }
    }
    std::cout << "Number of dirt blocks: " << dirtCount << std::endl;
    // count number of water blocks
    int waterCount = 0;
    for (auto object : objectManager->GetObjects())
    {
        if (object.second->GetID() == "water_block")
        {
            waterCount++;
        }
    }
    std::cout << "Number of water blocks: " << waterCount << std::endl;

    // count number of face we are rendering
    int faceCount = 0;
    for (auto object : objectManager->GetObjects())
    {
        faceCount += object.second->GetComponent<MeshComponent>()->GetVisibleSides().size();
    }
    std::cout << "Number of faces: " << faceCount << std::endl;
}
