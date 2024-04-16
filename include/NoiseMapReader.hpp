#ifndef NOISE_MAP_READER_HPP
#define NOISE_MAP_READER_HPP

#include <vector>
#include <string>

#include "ObjectManager.hpp"
#include "TextureManager.hpp"

class NoiseMapReader {
public:
    // Constructor that takes the path to the CSV file
    explicit NoiseMapReader(const std::string& filename);

    // Method to read the noise map from the CSV file
    void readNoiseMap();

    void GenerateWorld(std::shared_ptr<ObjectManager> objectManager, const std::shared_ptr<TextureManager>& textureManager);

    // Getter for the noise map
    const std::vector<std::vector<double>>& getNoiseMap() const;

private:
    std::string filename;
    std::vector<std::vector<double>> noiseMap;
};

#endif // NOISE_MAP_READER_HPP
