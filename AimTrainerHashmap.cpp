#include "AimTrainerHashmap.h"

hashmap::hashmap() {
}

// Function to add a target to the hashmap
void hashmap::addTarget(Target* target) {
	std::string key = generateKey(target->getPos());
	if (target->isActive()) map[key] = target;
	else map.erase(key);
}

// Helper function to generate a key for the hashmap
std::string hashmap::generateKey(sf::Vector2f position) {
	int x_key = static_cast<int>(position.x) / grid_size;
	int y_key = static_cast<int>(position.y) / grid_size;
	return std::to_string(x_key) + "," + std::to_string(y_key);
}

// Helper function to check for target at a specific grid position
Target* hashmap::getTargetInGrid(const std::string& key, sf::Vector2f position) {
    if (map.count(key)) {
        Target* target = map.at(key);
        // Check if the position is actually within the target's radius
        if (distance(position, target->getPos()) <= target->getShape().getRadius()) {
            return target;
        }
    }
    return nullptr;
}

// Function to iterate over grid squares and find a collision
Target* hashmap::getTargetAtCollision(sf::Vector2f position, float radius) {
    // Iterate over the grid squares within the radius for a collision
    for (int x_offset = -radius; x_offset <= radius; x_offset += grid_size) {
        for (int y_offset = -radius; y_offset <= radius; y_offset += grid_size) {
            sf::Vector2f check_position = { position.x + x_offset, position.y + y_offset };
            std::string key = generateKey(check_position);
            Target* target = getTargetInGrid(key, position);
            if (target) {
                return target;
            }
        }
    }
    return nullptr;
}

// Helper function to calculate distance between two points
float hashmap::distance(sf::Vector2f a, sf::Vector2f b) {
    sf::Vector2f diff = a - b;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}
