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
    /// Concept to check if a class has a method getSprite that returns an sf::Sprite
    template <class T>
    concept HasGetSprite = requires(T t) {
        { t.getSprite() } -> std::convertible_to<sf::Sprite>;
    };

    template <HasGetSprite T>
    class Node;  // Forward declaration of Node to be used in QuadtreeObject

    template <HasGetSprite T>
    class QuadtreeObject {
    public:
        T* object; // Pointer to the actual object
        std::unordered_set<Node<T>*> registeredNodes; // Nodes that this object is registered with

        QuadtreeObject(T* obj) : object(obj) {}

        // Return the position using the underlying object's sprite
        sf::Vector2f getPosition() const {
            return object->getSprite().getPosition();
        }

        // Ensure compatibility with the quadtree by forwarding the getSprite method
        sf::Sprite getSprite() const {
            return object->getSprite();
        }

        // Additional methods to handle node registration could go here
        void registerNode(Node<T>* node) {
            registeredNodes.insert(node);
        }

        void unregisterNode(Node<T>* node) {
            registeredNodes.erase(node);
        }

        T* getObject() {
			return object;
		}
    };

    template <HasGetSprite T>
    class Node {
    public:
        sf::FloatRect boundary;
        std::vector<QuadtreeObject<T>*> objects;
        std::vector<std::unique_ptr<Node<T>>> children;
        bool divided;
        int maxObjects;

        Node(const sf::FloatRect& bounds, const int maxObjects) : boundary(bounds), divided(false), maxObjects(maxObjects) {}

        void update() {
            if (divided) {
                for (auto& child : children) {
                    child->update();
                }
            }

            if (!divided) return;

            int totalObjects = 0;
            for (const auto& child : children) {
                totalObjects += static_cast<int>(child->objects.size());
            }

            // If total objects in children are less than maxObjects and no child is divided, merge them
            if (totalObjects < maxObjects && std::none_of(children.begin(), children.end(), [](const auto& child) { return child->divided; })) {
                merge();
            }
        }

        void subdivide() {
            float x = boundary.left;
            float y = boundary.top;
            float width = boundary.width / 2;
            float height = boundary.height / 2;

            children.push_back(std::make_unique<Node<T>>(sf::FloatRect(x, y, width, height), maxObjects));
            children.push_back(std::make_unique<Node<T>>(sf::FloatRect(x + width, y, width, height), maxObjects));
            children.push_back(std::make_unique<Node<T>>(sf::FloatRect(x, y + height, width, height), maxObjects));
            children.push_back(std::make_unique<Node<T>>(sf::FloatRect(x + width, y + height, width, height), maxObjects));

            divided = true;

            // Redistribute objects to new children
            for (auto obj : objects) {
                for (auto& child : children) {
                    if (child->boundary.contains(obj->getSprite().getPosition())) {
                        child->objects.push_back(obj);
                        break;
                    }
                }
            }

            objects.clear();  // Clear the objects in the current node as they are now in the children
        }

        bool addObject(QuadtreeObject<T>* qtObject) {
            if (!boundary.contains(qtObject->getPosition())) {
                return false;
            }

            if (divided) {
                for (auto& child : children) {
                    if (child->addObject(qtObject)) {
                        return true;
                    }
                }
                return false;
            }

            if (objects.size() >= maxObjects) {
                subdivide();
                return addObject(qtObject);
            }

            objects.push_back(qtObject);
            qtObject->registerNode(this); // Tag the node
            return true;
        }

        bool removeObject(QuadtreeObject<T>* qtObject) {
            // Check current node's objects
            auto it = std::find_if(objects.begin(), objects.end(), [&](QuadtreeObject<T>* obj) {
                return obj == qtObject;
                });

            if (it != objects.end()) {
                objects.erase(it);
                qtObject->unregisterNode(this); // Update the node registration
                return true;
            }

            // Check children if not found
            if (divided) {
                for (auto& child : children) {
                    if (child->removeObject(qtObject)) {
                        return true;
                    }
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
        std::vector<T*> findObjectsNearObject(U* queryObject, float distance) {
            sf::FloatRect queryBounds = queryObject->getSprite().getGlobalBounds();
            queryBounds.left -= distance;
            queryBounds.top -= distance;
            queryBounds.width += 2 * distance;
            queryBounds.height += 2 * distance;

            if (!boundary.intersects(queryBounds)) return std::vector<T*>();

            std::vector<T*> found;
            findObjectsInRange(queryBounds, found, distance, queryObject);

            return found;
        }

        template <HasGetSprite U>
        void findObjectsInRange(const sf::FloatRect& range, std::vector<T*>& found, float distance, U* queryObject) {
            // Check objects in the current node
            for (auto object : objects) {
                if (range.intersects(object->getSprite().getGlobalBounds())) {
                    if (vm::distance(object->getSprite().getPosition(), queryObject->getSprite().getPosition()) <= distance) {
                        found.push_back(object->getObject());
                    }
                }
            }

            // Recur into children if the node is divided
            if (divided) {
                for (auto& child : children) {
                    if (child->boundary.intersects(range)) {
                        child->findObjectsInRange(range, found, distance, queryObject);
                    }
                }
            }
        }


        bool updateObjectPosition(QuadtreeObject<T>* object) {
            // Check if the sprite is still within the boundary
            if (boundary.intersects(object->getSprite().getGlobalBounds())) {
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
            if (!divided) {
                GlobalValues::getInstance().displayText("Num obj: " + std::to_string(objects.size()), sf::Vector2f(boundary.left + boundary.width / 2 - 10.f, boundary.top + boundary.height / 2 - 5.f), sf::Color::White);
            }

            if (divided) {
                for (auto& child : children) {
                    child->draw(window);
                }
            }

            sf::RectangleShape rectangle(sf::Vector2f(boundary.width, boundary.height));
            rectangle.setPosition(boundary.left, boundary.top);
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineThickness(5.f);
            rectangle.setOutlineColor(sf::Color::Blue); // Node boundary

            window->draw(rectangle);

            // Draw the objects' positions
            for (auto object : objects) {
                sf::CircleShape marker(10); // Small circle marker
                sf::Vector2f pos = object->getSprite().getPosition();
                marker.setPosition(pos.x, pos.y);
                marker.setFillColor(sf::Color::Magenta); // Object position
                window->draw(marker);
            }
        }
    };

    /// Quadtree class, this will be the main class that will be used to create the quadtree.
    template <HasGetSprite T>
    class Quadtree {
    public:
        const int maxObjects = 4;
        std::unique_ptr<Node<T>> root;
        std::unordered_map<T*, QuadtreeObject<T>*> objectMap;

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

            root = std::make_unique<Node<T>>(initialBoundary, maxObjects);
        }

        void update() {
            root->update();
        }

        void addObject(T* object) {
            auto qtObject = new QuadtreeObject<T>(object);
            if (root->addObject(qtObject)) {
                objectMap[object] = qtObject; // Store the mapping
            }
        }

        void removeObject(T* object) {
            auto it = objectMap.find(object);
            if (it != objectMap.end()) {
                root->removeObject(it->second);
                objectMap.erase(it); // Remove the mapping
                delete it->second; // Clean up the QuadtreeObject
            }
        }

        template <HasGetSprite U>
        std::vector<T*> findObjectsNearObject(U* queryObject, float distance) {
            return root->findObjectsNearObject(queryObject, distance);
        }

        bool updateObjectPosition(T* object) {
            // First remove the object from its current node
            if (!root->removeObject(objectMap[object])) {
                // If removeObject returns false, the object was not found in the current boundary
                return false;
            }
            // Try to add the object starting from the root node
            return root->addObject(objectMap[object]);
        }

        void draw(sf::RenderWindow* window) {
            root->draw(window);
        }
    };
}
