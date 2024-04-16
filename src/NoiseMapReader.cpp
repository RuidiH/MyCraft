#include "NoiseMapReader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

#include "TransformComponent.hpp"
#include "MeshComponent.hpp"
#include "TextureComponent.hpp"
#include "Component.hpp"

// Constructor
NoiseMapReader::NoiseMapReader(const std::string& filename) : filename(filename) {}

// Method to read the noise map
void NoiseMapReader::readNoiseMap() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::istringstream sstream(line);
        std::string field;
        while (getline(sstream, field, ',')) {
            row.push_back(std::stod(field));
        }
        noiseMap.push_back(row);
    }

    file.close();
}

// Getter for the noise map
const std::vector<std::vector<double>>& NoiseMapReader::getNoiseMap() const {
    return noiseMap;
}

void NoiseMapReader::GenerateWorld(std::shared_ptr<ObjectManager> objectManager, const std::shared_ptr<TextureManager>& textureManager) {

    int x_start = -1 *noiseMap.size() / 2;
    int z_start = -1 * noiseMap[0].size() / 2;
    int y_start = -5;

    // generate base dirt layer
    for (int i = 0; i < noiseMap.size(); i++) {
        for (int j = 0; j < noiseMap[i].size(); j++) {

            int xPosition = x_start + i;
            int zPosition = z_start + j;

            int yPosition = noiseMap[i][j] * 10 / 2; 

            // dirt
            for (int k = -2; k < yPosition - 2; k++) {
                auto cube = std::make_shared<GameObject>("dirt_block");
                cube->AddComponent<TransformComponent>(glm::vec3(xPosition, k, zPosition));
                cube->AddComponent<MeshComponent>()->AddMesh(MeshType::CUBE);
                cube->AddComponent<TextureComponent>()->SetTextureGroupName("dirt");
                cube->GetComponent<TextureComponent>()->SetTextureGroup(textureManager->GetTextureGroup("dirt"));
                objectManager->AddObject(cube);

                // grass 
                if (k == yPosition - 3 && k > -1) {
                    auto cube = std::make_shared<GameObject>("grass_block");
                    cube->AddComponent<TransformComponent>(glm::vec3(xPosition, k, zPosition));
                    cube->AddComponent<MeshComponent>()->AddMesh(MeshType::CUBE);
                    cube->AddComponent<TextureComponent>()->SetTextureGroupName("grass");
                    cube->GetComponent<TextureComponent>()->SetTextureGroup(textureManager->GetTextureGroup("grass"));
                    objectManager->AddObject(cube);
                }
            }

            // water
            for (int k = yPosition - 2; k < 0; k++) {
                auto cube = std::make_shared<GameObject>("water_block");
                cube->AddComponent<TransformComponent>(glm::vec3(xPosition, k, zPosition));
                cube->AddComponent<MeshComponent>()->AddMesh(MeshType::WATER);
                objectManager->AddObject(cube);
            }
        }
    }
}
