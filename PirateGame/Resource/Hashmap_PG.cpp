#include "Hashmap_PG.h"

using namespace PirateGame;

Hashmap::Hashmap() {
}

// Generate a key for the hashmap based on position
std::pair<int, int> Hashmap::generateKey(sf::Vector2f pos) {
	return { pos.x, pos.y };
}

// Add a new object to the hashmap
void Hashmap::addLandMass(LandMass* landmass) {
    // Use the bounding box of the sprite for grid calculations
    sf::FloatRect bounds = landmass->getSprite().getGlobalBounds();
    auto topLeft = map.getGridCoordinates(bounds.left, bounds.top);
    auto bottomRight = map.getGridCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);

    for (int i = topLeft.first; i <= bottomRight.first; ++i) {
        for (int j = topLeft.second; j <= bottomRight.second; ++j) {
            std::pair<int, int> key = generateKey(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));
            this->hashmap[key] = landmass;
        }
    }
}

// Remove an object from the hashmap
void Hashmap::removeLandMass(LandMass* landmass) {
	// Similar to addLandMass, you need to remove the landmass from all the cells it occupies
	auto topLeft = map.getGridCoordinates(landmass->getSpritePos().x, landmass->getSpritePos().y);
	auto bottomRight = map.getGridCoordinates(landmass->getSpritePos().x + landmass->getSpriteSize().x,
		landmass->getSpritePos().y + landmass->getSpriteSize().y);

	for (int i = topLeft.first; i <= bottomRight.first; ++i) {
		for (int j = topLeft.second; j <= bottomRight.second; ++j) {
			std::pair<int, int> key = generateKey(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));
			this->hashmap.erase(key);
		}
	}
}

// Find landmass near to a player
std::set<LandMass*> Hashmap::findLandMassNearPlayer(Ship& ship, sf::RenderWindow& window, bool debug) {
    // Get the global bounds of the player ship's sprite
    sf::FloatRect shipBounds = ship.getSprite().getGlobalBounds();

    // Use the bounds to calculate the extended area around the ship for finding nearby landmasses
    float left = shipBounds.left - shipBounds.width;
    float right = shipBounds.left + 2 * shipBounds.width;
    float top = shipBounds.top - shipBounds.height;
    float bottom = shipBounds.top + 2 * shipBounds.height;

    // Get the top left and bottom right cells of the bounding box
    auto topLeft = map.getGridCoordinates(left, top);
    auto bottomRight = map.getGridCoordinates(right, bottom);

    std::set<LandMass*> landmasses;

    // Check the cells in the bounding box
    for (int i = topLeft.first; i <= bottomRight.first; i++) {
        for (int j = topLeft.second; j <= bottomRight.second; j++) {
            std::pair<int, int> key = generateKey(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));

            // If the key exists in the hashmap, add the landmass to the set
            if (hashmap.count(key)) {
                landmasses.insert(hashmap.at(key));
            }

            if (!debug) {
				continue;
			}

            // Optionally, visualize the grid cells being checked (for debugging)
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(static_cast<float>(map.getCellSize()), static_cast<float>(map.getCellSize())));
            rect.setFillColor(sf::Color::Magenta);  // Color for visualization
            rect.setPosition(sf::Vector2f(i * static_cast<float>(map.getCellSize()), j * static_cast<float>(map.getCellSize())));  // Adjust position based on your grid sizing
            window.draw(rect);
        }
    }

    return landmasses;
}

