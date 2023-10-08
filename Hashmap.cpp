#include "Hashmap.h"

Hashmap::Hashmap(Map& map, sf::RenderWindow& window) {
	// Initialize the hashmap with the number of cells in the grid
	int numCellsX = window.getSize().x / map.getCellSize();  // Calculate the number of cells in the x direction
	int numCellsY = window.getSize().y / map.getCellSize(); // Calculate the number of cells in the y direction
	for (int i = 0; i < numCellsX; i++) {              // Iterate through the cells in the x direction
    		for (int j = 0; j < numCellsY; j++) {          // Iterate through the cells in the y direction
            			std::string key = generateKey(i, j);       // Generate a key based on the grid indices
            			hashmap[key] = {};                         // Initialize the vector at the key to be empty
            		}
    	}
}

void Hashmap::assignPellet(const Pellet& pellet, Map& map) {
    float x = pellet.getPosition().x;           
    float y = pellet.getPosition().y;             
    int gridX = x / map.getCellSize();            // Convert x coordinate to grid index
    int gridY = y / map.getCellSize();            // Convert y coordinate to grid index
    std::string key = generateKey(gridX, gridY);  // Generate a key based on the grid indices
    GridObject gridObject{ pellet };              // Create a GridObject with the pellet
    hashmap[key].push_back(gridObject);           // Add the GridObject to the hashmap at the generated key
}

void Hashmap::assignCircle(const Circle& circle, Map& map) {
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

std::vector<Pellet> Hashmap::getPelletsInSameCell(float x, float y, Map& map) const {
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

void Hashmap::checkCollision(Circle& circle, Hashmap& hashmap, Map& map, sf::RenderWindow& window) {
    float x = circle.getPosition().x;                  
    float y = circle.getPosition().y;                    
    std::vector<Pellet> nearbyPellets = hashmap.getPelletsInSameCell(x, y, map);  // Get the pellets in the same cell as the circle
    for (auto& pellet : nearbyPellets) {           // Iterate through the nearby pellets
        float dx = pellet.getPosition().x - x;           // Calculate the x-distance between the circle and the pellet
        float dy = pellet.getPosition().y - y;           // Calculate the y-distance between the circle and the pellet
        float distanceSquared = dx * dx + dy * dy;       // Calculate the squared distance between the circle and the pellet
        float sumRadii = circle.getCirclesize() + pellet.getRadius();  // Calculate the sum of the radii of the circle and pellet
        if (distanceSquared <= sumRadii * sumRadii) {    // Check for a collision by comparing the squared distance to the square of the sum of the radii
            pellet.RemovePellet(window);
        }
        else {

        }
    }
}
