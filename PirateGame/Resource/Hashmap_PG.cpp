#include "Hashmap_PG.h"

HashmapPG::HashmapPG(Map& map_in) : map(map_in) {
}

// Generate a key for the hashmap based on position
std::string HashmapPG::generateKey(sf::Vector2f pos) {
	return std::to_string(static_cast<int>(pos.x)) + "," + std::to_string(static_cast<int>(pos.y));
}

// Add a new object to the hashmap
void HashmapPG::addLandMass(LandMass* landmass) {
	// Calculate the range of cells occupied by the landmass
	auto topLeft = map.getGridCoordinates(landmass->getPos().x, landmass->getPos().y);
	auto bottomRight = map.getGridCoordinates(landmass->getPos().x + landmass->getShape().getSize().x,
		landmass->getPos().y + landmass->getShape().getSize().y);

	for (int i = topLeft.first; i <= bottomRight.first; ++i) {
		for (int j = topLeft.second; j <= bottomRight.second; ++j) {
			std::string key = generateKey(sf::Vector2f(i, j));
			this->hashmap[key] = landmass;
		}
	}
}

// Remove an object from the hashmap
void HashmapPG::removeLandMass(LandMass* landmass) {
	// Similar to addLandMass, you need to remove the landmass from all the cells it occupies
	auto topLeft = map.getGridCoordinates(landmass->getPos().x, landmass->getPos().y);
	auto bottomRight = map.getGridCoordinates(landmass->getPos().x + landmass->getShape().getSize().x,
		landmass->getPos().y + landmass->getShape().getSize().y);

	for (int i = topLeft.first; i <= bottomRight.first; ++i) {
		for (int j = topLeft.second; j <= bottomRight.second; ++j) {
			std::string key = generateKey(sf::Vector2f(i, j));
			this->hashmap.erase(key);
		}
	}
}

// Find landmass near to a player
std::set<LandMass*> HashmapPG::findLandMassNearPlayer(Ship& ship, sf::RenderWindow& window) {
	// Get the bouding box of the player ship
	float left = ship.getPosition().x - 2 * ship.getSize().x;
	float right = ship.getPosition().x + 2 * ship.getSize().x;
	float top = ship.getPosition().y - 2 * ship.getSize().y;
	float bottom = ship.getPosition().y + 2 * ship.getSize().y;

	// Get the top left and bottom right cells of the bounding box
	auto topLeft = map.getGridCoordinates(left, top);
	auto bottomRight = map.getGridCoordinates(right, bottom);

	// Create a set to store the landmasses found
	std::set<LandMass*> landmasses;
	
	// Check the cells in the bounding box
	for (int i = topLeft.first; i <= bottomRight.first; i++) {
		for (int j = topLeft.second; j <= bottomRight.second; j++) {
			// Get the key for the hashmap
			std::string key = generateKey(sf::Vector2f(i, j));

			// If the key exists in the hashmap, add the landmass to the set
			if (hashmap.count(key)) {
				landmasses.insert(hashmap.at(key));
			}

			// Set the color of the cells being checked
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(map.getCellSize(), map.getCellSize()));
			rect.setFillColor(sf::Color::Magenta);
			rect.setPosition(sf::Vector2f(i, j) * 100.f);
			window.draw(rect);
		}
	}

	return landmasses;
}
