#include "Hashmap.h"
#include "Pellet.h"
#include "Globals.h"
#include "World.h"


void Hashmap::assignPellet(const Pellet& pellet) {
    float x = pellet.getPosition().x;           
    float y = pellet.getPosition().y;             
    int gridX = x / map.getCellSize();            // Convert x coordinate to grid index
    int gridY = y / map.getCellSize();            // Convert y coordinate to grid index
    std::string key = generateKey(gridX, gridY);  // Generate a key based on the grid indices
    GridObject gridObject{ pellet };              // Create a GridObject with the pellet
    hashmap[key].push_back(gridObject);           // Add the GridObject to the hashmap at the generated key
}

void Hashmap::assignCircle(const Circle& circle) {
    float x = circle.getPosition().x;              
    float y = circle.getPosition().y;             
    int gridX = x / map.getCellSize();            
    int gridY = y / map.getCellSize();           
    std::string key = generateKey(gridX, gridY); 
    GridObject gridObject{ circle };              
    hashmap[key].push_back(gridObject);           
}

std::string Hashmap::generateKey(int x, int y) const {
    return std::to_string(x) + "," + std::to_string(y);  // Concatenate the grid indices to form a string key
}

std::vector<Pellet> Hashmap::getPelletsInSameCell(float x, float y) const {
    int gridX = x / map.getCellSize();                    // Convert x coordinate to grid index
    int gridY = y / map.getCellSize();                    // Convert y coordinate to grid index
    std::string key = generateKey(gridX, gridY);          // Generate a key based on the grid indices
    auto it = hashmap.find(key);                          // Look for the key in the hashmap
    if (it != hashmap.end()) {                            // If the key is found in the hashmap
        std::vector<Pellet> pellets;
        for (const auto& gridObject : it->second) {      // Iterate through the grid objects at the found key
            if (auto pelletPtr = std::get_if<Pellet>(&gridObject.object)) {  // If the grid object is a pellet, add it to the vector
                pellets.push_back(*pelletPtr);
            }
        }
        return pellets;                                   // Return the vector of pellets
    }
    return {};                                            // If the key is not found in the hashmap, return an empty vector
}

bool Hashmap::checkCollision(Circle& circle, Pellet& pellet, Hashmap hashmap, World world) {
    float x = circle.getPosition().x;                  
    float y = circle.getPosition().y;                    
    std::vector<Pellet> nearbyPellets = hashmap.getPelletsInSameCell(x, y);  // Get the pellets in the same cell as the circle
    std::vector<std::vector<int>> pelletsToBeDeleted(nearbyPellets.size(), std::vector<int>(2, 0));
    for (const auto& pellet : nearbyPellets) {           // Iterate through the nearby pellets
        float dx = pellet.getPosition().x - x;           // Calculate the x-distance between the circle and the pellet
        float dy = pellet.getPosition().y - y;           // Calculate the y-distance between the circle and the pellet
        float distanceSquared = dx * dx + dy * dy;       // Calculate the squared distance between the circle and the pellet
        float sumRadii = circle.getCirclesize() + pellet.getRadius();  // Calculate the sum of the radii of the circle and pellet
        if (distanceSquared <= sumRadii * sumRadii) {    // Check for a collision by comparing the squared distance to the square of the sum of the radii
            world.RemovePelletWhenCollision(pelletsToBeDeleted);
        }
        else {
            return false;
        }
    }
}
