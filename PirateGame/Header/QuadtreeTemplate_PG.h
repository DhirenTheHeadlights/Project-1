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
	/// Node stuct, the quadtree will be made up of these. Each node will have its own boundary, objects, children, and divided status.
	template <class T> // This is a template type 'T' to specify the object type that makes up the quadtree
	concept HasGetSprite = requires(T t) {
		{ t.getSprite() } -> std::convertible_to<sf::Sprite>;
	};
	template <HasGetSprite T>

	struct Node {
		sf::FloatRect boundary;
		std::vector<T*> objects;
		std::vector<std::unique_ptr<Node>> children;
		bool divided;

		Node(const sf::FloatRect& bounds) : boundary(bounds), divided(false) {}

		void subdivide() {
			float x = boundary.left;
			float y = boundary.top;
			float width = boundary.width / 2;
			float height = boundary.height / 2;

			children.push_back(std::make_unique<Node>(sf::FloatRect(x, y, width, height)));
			children.push_back(std::make_unique<Node>(sf::FloatRect(x + width, y, width, height)));
			children.push_back(std::make_unique<Node>(sf::FloatRect(x, y + height, width, height)));
			children.push_back(std::make_unique<Node>(sf::FloatRect(x + width, y + height, width, height)));

			divided = true;
		}

		bool addObject(T* object) {
			if (!boundary.contains(object->getSprite().getPosition())) {
				return false;
			}

			if (objects.size() < 4 && !divided) {
				objects.push_back(object);
				std::cout << "Object added to node" << std::endl;
				return true;
			}

			if (!divided) {
				subdivide();
				std::cout << "Node divided" << std::endl;
			}

			for (auto& child : children) {
				if (child->addObject(object)) return true;
			}

			return false;
		}

		bool removeObject(T* object) {
			if (!boundary.contains(object->getSprite().getPosition())) return false;

			if (objects.size() > 0) {
				auto it = std::find(objects.begin(), objects.end(), object);
				if (it != objects.end()) {
					objects.erase(it);
					return true;
				}
			}

			if (divided) {
				for (auto& child : children) {
					if (child->removeObject(object)) return true;
				}
			}

			return false;
		}

		void merge() {
			if (divided) {
				for (auto& child : children) {
					child->merge();
					objects.insert(objects.end(), child->objects.begin(), child->objects.end());
				}
				children.clear();
				divided = false;
			}
		}

		template <HasGetSprite U> 
		std::vector<T*> findObjectsNearObject(U* queryObject) {
			sf::FloatRect range = queryObject->getSprite().getGlobalBounds();

			std::vector<T*> found;
			if (!boundary.intersects(range)) return found;

			for (auto object : objects) {
				if (range.intersects(object->getSprite().getGlobalBounds())) {
					found.push_back(object);
				}
			}

			if (divided) {
				for (auto& child : children) {
					auto childFound = child->findObjectsNearObject(queryObject);
					found.insert(found.end(), childFound.begin(), childFound.end());
				}
			}

			return found;
		}

		bool updateObjectPosition(T* object) {
			sf::Vector2f currentPosition = object->getSprite().getPosition();
			sf::FloatRect spriteBounds = object->getSprite().getGlobalBounds();
			spriteBounds.left = currentPosition.x - spriteBounds.width / 2;
			spriteBounds.top = currentPosition.y - spriteBounds.height / 2;

			// Check if the new position is within the same node
			if (boundary.contains(currentPosition)) {
				// If already within the same node and correct position, no need to do anything
				return true;
			}
			else {
				// If the position is outside the current node's boundary, remove and reinsert the object
				if (removeObject(object)) {
					return addObject(object);
				}
			}
			return false;
		}

		// Method to draw the node boundaries for debugging
		void draw(sf::RenderWindow* window) {
			if (divided) {
				for (auto& child : children) {
					child->draw(window);
				}
			}

			sf::RectangleShape rectangle(sf::Vector2f(boundary.width, boundary.height));
			rectangle.setPosition(boundary.left, boundary.top);
			rectangle.setFillColor(sf::Color::Transparent);
			rectangle.setOutlineThickness(5.f);
			rectangle.setOutlineColor(sf::Color::Green); // Node boundary

			window->draw(rectangle);

			// Draw the objects' positions
			for (auto object : objects) {
				sf::CircleShape marker(10); // Small circle marker
				sf::Vector2f pos = object->getSprite().getPosition();
				marker.setPosition(pos.x - 5, pos.y - 5); // Center the marker on the object's position
				marker.setFillColor(sf::Color::Red); // Object position
				window->draw(marker);
			}
		}

	};

	/// Quadtree class, this will be the main class that will be used to create the quadtree.
	template <HasGetSprite T>
	class Quadtree {
	public:
		std::unique_ptr<Node<T>> root;

		Quadtree() {
			std::vector<Map*> maps = GlobalMap::getInstance().getAllChunks();

			// Find the boundaries as the minimum and maximum x and y values
			sf::FloatRect initialBoundary = maps.front()->getBounds();
			for (auto map : maps) {
				sf::FloatRect mapBounds = map->getBounds();
				if (mapBounds.left < initialBoundary.left) initialBoundary.left = mapBounds.left;
				if (mapBounds.top < initialBoundary.top) initialBoundary.top = mapBounds.top;
				if (mapBounds.left + mapBounds.width > initialBoundary.left + initialBoundary.width) initialBoundary.width = mapBounds.left + mapBounds.width - initialBoundary.left;
				if (mapBounds.top + mapBounds.height > initialBoundary.top + initialBoundary.height) initialBoundary.height = mapBounds.top + mapBounds.height - initialBoundary.top;
			}

			std::cout << "Initial boundary: " << initialBoundary.left << ", " << initialBoundary.top << ", " << initialBoundary.width << ", " << initialBoundary.height << std::endl;

			root = std::make_unique<Node<T>>(initialBoundary);
		}

		void addObject(T* object) {
			root->addObject(object);
		}

		void removeObject(T* object) {
			root->removeObject(object);
		}

		template <HasGetSprite U>
		std::vector<T*> findObjectsNearObject(U* queryObject, float range = 0.f /* does nothing */) {
			return root->findObjectsNearObject(queryObject);
		}

		bool updateObjectPosition(T* object) {
			return root->updateObjectPosition(object);
		}

		void draw(sf::RenderWindow* window) {
			root->draw(window);
		}
	};
}