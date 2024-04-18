import numpy as np
from noise import pnoise2
import matplotlib.pyplot as plt
import csv  # Import the csv module

# Settings for the noise map
width = 40
height = 40
scale = 0.1  # Scale of the noise
octaves = 6  # Number of octaves
persistence = 0.5
lacunarity = 2.0
seed = np.random.randint(0, 100)

# Generate the noise map
noise_map = np.zeros((height, width))
for y in range(height):
    for x in range(width):
        noise_value = pnoise2(x * scale, 
                              y * scale,
                              octaves=octaves,
                              persistence=persistence,
                              lacunarity=lacunarity,
                              repeatx=width,
                              repeaty=height,
                              base=seed)
        noise_map[y][x] = noise_value

# Display the noise map using matplotlib
# plt.figure(figsize=(8, 8))
# plt.imshow(noise_map, cmap='gray')
# plt.colorbar()
# plt.title('50x50 Perlin Noise Map')
# plt.show()

# Normalize the noise map
noise_map = (noise_map - np.min(noise_map)) / (np.max(noise_map) - np.min(noise_map))

# Save the noise map to a CSV file
with open('noise_map.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    for row in noise_map:
        writer.writerow(row)

# Print message when done
print("Noise map has been saved to 'noise_map.csv'")