#pragma once

/// This is a template class for a hashmap that will store pointers to objects.
/// Each object needs to have a sprite to grab a position from.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include "VectorMath.h"

#include "GlobalValues_PG.h"
#include "GlobalMap_PG.h"

namespace PirateGame {
	// Template class
	template <class T>
	/// Here, because of C++ 20, we can use concepts to make sure that the class T has a function called getSprite() that returns a sf::Sprite
	concept HasGetSprite = requires(T t) {
		{ t.getSprite() } -> std::convertible_to<sf::Sprite>;
	};
	template <HasGetSprite T>

	class Hashmap {
	public:
		Hashmap() {};

		// Generate key for hashmap
		std::pair<int, int> generateKey(sf::Vector2f position) {
			return { static_cast<int>(position.x), static_cast<int>(position.y) };
		}

		// Add and remove an object to the hashmap
		void addObject(T* object) {
			// Grab map at chunk
			Map* map = GlobalMap::getInstance().getMapAtCurrentChunk(object->getSprite().getPosition());

			// Use the bounding box of the sprite for grid calculations
			sf::FloatRect bounds = object->getSprite().getGlobalBounds();
			auto topLeft = map->getGridCoordinates(bounds.left, bounds.top);
			auto bottomRight = map->getGridCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);

			std::unordered_set<std::pair<int, int>, pair_hash> occupiedPositions;

			for (int i = topLeft.first; i <= bottomRight.first; ++i) {
				for (int j = topLeft.second; j <= bottomRight.second; ++j) {
					std::pair<int, int> key = generateKey(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));
					this->hashmap[key] = object;
					occupiedPositions.insert(key); // Store positions for the reverse mapping
				}
			}

			// Add the object to the reverse hashmap
			this->reverseHashmap[object] = occupiedPositions;
		}

		void removeObject(T* object) {
			// Fetch the objects's current positions from the reverse mapping
			if (this->reverseHashmap.find(object) != this->reverseHashmap.end()) {
				const std::unordered_set<std::pair<int, int>, pair_hash>& occupiedPositions = this->reverseHashmap[object];

				for (const auto& pos : occupiedPositions) {
					this->hashmap.erase(pos); // Remove the object from these positions in the primary hashmap
				}

				// Remove the objects's entry from the reverse mapping
				this->reverseHashmap.erase(object);
			}
		}

		/// Here, we are going to use another template type 'U' to specify the object type that is searching in the hashmap
		/// This template type 'U' also needs to have a function called getSprite() that returns a sf::Sprite
		template <HasGetSprite U>

		// Find object near to another object
		std::set<T*> findObjectsNearObject(U* object, float maxDistance = 1000.f, bool debug = false) {
			// Grab window and chunk
			sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
			Map* map = GlobalMap::getInstance().getMapAtCurrentChunk(object->getSprite().getPosition());

			maxDistance /= 2; // Divide by 2 to get the radius

			// Get the position of the object
			sf::Vector2f objectPosition = object->getSprite().getPosition();

			// Get the top left and bottom right cells of the bounding box
			auto topLeft = map->getGridCoordinates(objectPosition.x - maxDistance, objectPosition.y - maxDistance);
			auto bottomRight = map->getGridCoordinates(objectPosition.x + maxDistance, objectPosition.y + maxDistance);

			// Create a set to store the nearby objects
			std::set<T*> nearbyObjects;

			// Loop through the cells in the bounding box
			for (int i = topLeft.first; i <= bottomRight.first; ++i) {
				for (int j = topLeft.second; j <= bottomRight.second; ++j) {
					std::pair<int, int> key = generateKey(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));
					// Check if the key exists in the hashmap
					if (this->hashmap.find(key) != this->hashmap.end()) {

						// Grab the object from the hashmap
						T* otherObj = this->hashmap[key];
										 
						// Get the position of the object
						sf::Vector2f otherObjPos = otherObj->getSprite().getPosition();
										 
						// Calculate the distance between the two objects
						float distance = vm::distance(objectPosition, otherObjPos);
										 
						// Check if the distance is less than the max distance
						if (distance <= maxDistance) {
							// Add the object to the set
							nearbyObjects.insert(otherObj);
						}
					}
				}
			}
										 
			// If debug is true, draw the grid cells being checked
			if (debug) {
				// Draw the grid cells being checked
				for (int i = topLeft.first; i <= bottomRight.first; ++i) {
					for (int j = topLeft.second; j <= bottomRight.second; ++j) {
						sf::RectangleShape rect(sf::Vector2f(static_cast<float>(map->getCellSize()), static_cast<float>(map->getCellSize())));
						rect.setPosition(static_cast<float>(i) * static_cast<float>(map->getCellSize()), static_cast<float>(j) * static_cast<float>(map->getCellSize()));
						rect.setFillColor(sf::Color::Transparent);
						rect.setOutlineColor(sf::Color::Red);
						rect.setOutlineThickness(1.f);
						window->draw(rect);
					}
				}
			}

			// Return the set of nearby objects
			return nearbyObjects;
		}

		// Update the object in the hashmap
		void updateObjectPosition(T* object) {
			// Grab map at chunk
			Map* map = GlobalMap::getInstance().getMapAtCurrentChunk(object->getSprite().getPosition());

			// Get the object's new position from its sprite and calculate new grid cells it occupies
			sf::FloatRect bounds = object->getSprite().getGlobalBounds();
			auto newTopLeft = map->getGridCoordinates(bounds.left, bounds.top);
			auto newBottomRight = map->getGridCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);

			std::unordered_set<std::pair<int, int>, pair_hash> newPositions;
			for (int i = newTopLeft.first; i <= newBottomRight.first; ++i) {
				for (int j = newTopLeft.second; j <= newBottomRight.second; ++j) {
					newPositions.insert({ i, j });
				}
			}

			// Retrieve the object's current positions from the reverse hashmap
			const auto& currentPositions = reverseHashmap[object];

			std::unordered_set<std::pair<int, int>, pair_hash> positionsToAdd;
			std::unordered_set<std::pair<int, int>, pair_hash> positionsToRemove;

			// Determine which new positions need to be added (those not already in currentPositions)
			for (const auto& newPos : newPositions) {
				if (currentPositions.find(newPos) == currentPositions.end()) {
					positionsToAdd.insert(newPos);
				}
			}

			// Determine which positions need to be removed (those not in newPositions)
			for (const auto& pos : currentPositions) {
				if (newPositions.find(pos) == newPositions.end()) {
					positionsToRemove.insert(pos);
				}
			}

			// Update the hashmap and reverseHashmap
			for (const auto& pos : positionsToRemove) {
				hashmap.erase(pos); // Remove the object from old positions in the primary hashmap
			}
			for (const auto& newPos : positionsToAdd) {
				hashmap[newPos] = object; // Add or update the object in new positions in the primary hashmap
				reverseHashmap[object].insert(newPos); // Update the reverse hashmap
			}

			// Update the object's current positions in the reverseHashmap
			// This could be optimized by directly modifying the set, but for clarity, we're simply replacing it here
			reverseHashmap[object] = newPositions;
		}
	private:
		// A custom hash function for std::pair<int, int>
		struct pair_hash {
			std::size_t operator()(const std::pair<int, int>& pair) const {
				std::size_t h1 = std::hash<int>()(pair.first);
				std::size_t h2 = std::hash<int>()(pair.second);
				h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2); // Use a magic number from the golden ratio
				return h1;
			}
		};

		// Hashmap
		std::unordered_map<std::pair<int, int>, T*, pair_hash> hashmap;
		std::unordered_map<T*, std::unordered_set<std::pair<int, int>, pair_hash>> reverseHashmap;
	};
}