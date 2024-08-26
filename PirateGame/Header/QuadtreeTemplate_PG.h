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
#include "GlobalIDManager_PG.h"

namespace PirateGame {
    /// Concept to check if a class has a method getSprite that returns an sf::Sprite
    template <class T>
    concept HasGetID = requires(T t) {
        { t.getID() } -> std::convertible_to<ID*>;
    };

    template <class T>
    concept HasGetSprite = requires(T t) {
        { t.getSprite() } -> std::convertible_to<sf::Sprite>;
    };

    class Node;  // Forward declaration of Node to be used in QuadtreeObject

    class QuadtreeObject {
    public:
        ID* id;                                     // ID of the object
        sf::Sprite sprite;                          // Sprite of the object
        std::unordered_set<Node*> registeredNodes;  // Nodes that this object is registered with

        QuadtreeObject(ID* id, const sf::Sprite& sprite) : id(id), sprite(sprite) {}

        // Additional methods to handle node registration could go here
        void registerNode(Node* node) {
            registeredNodes.insert(node);
        }

        void unregisterNode(Node* node) {
            registeredNodes.erase(node);
        }
    };

    class Node {
    public:
        sf::FloatRect boundary;
        std::vector<QuadtreeObject*> objects;
        std::vector<std::unique_ptr<Node>> children;
        bool divided;

        Node(const sf::FloatRect& bounds, const int maxObjects) : boundary(bounds), divided(false) {
            objects.reserve(maxObjects);
        }

        void update(const int maxObjects) {
            if (divided) {
                for (auto& child : children) {
                    child->update(maxObjects);
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

        void subdivide(const int maxObjects) {
            float x = boundary.left;
            float y = boundary.top;
            float width = boundary.width / 2;
            float height = boundary.height / 2;

            children.push_back(std::make_unique<Node>(sf::FloatRect(x, y, width, height), maxObjects));
            children.push_back(std::make_unique<Node>(sf::FloatRect(x + width, y, width, height), maxObjects));
            children.push_back(std::make_unique<Node>(sf::FloatRect(x, y + height, width, height), maxObjects));
            children.push_back(std::make_unique<Node>(sf::FloatRect(x + width, y + height, width, height), maxObjects));

            divided = true;

            // Redistribute objects to new children
            for (auto obj : objects) {
                for (auto& child : children) {
                    if (child->boundary.contains(obj->sprite.getPosition())) {
                        child->objects.push_back(obj);
                        break;
                    }
                }
            }

            objects.clear();  // Clear the objects in the current node as they are now in the children
        }

        bool addObject(QuadtreeObject* qtObject, const int maxObjects) {
            if (!boundary.contains(qtObject->sprite.getPosition())) {
                return false;
            }

            if (divided) {
                for (auto& child : children) {
                    if (child->addObject(qtObject, maxObjects)) {
                        return true;
                    }
                }
                return false;
            }

            if (objects.size() >= maxObjects) {
                subdivide(maxObjects);
                return addObject(qtObject, maxObjects);
            }

            objects.push_back(qtObject);
            qtObject->registerNode(this); // Tag the node
            return true;
        }

        bool removeObject(QuadtreeObject* qtObject) {
            // Check current node's objects
            auto it = std::find_if(objects.begin(), objects.end(), [&](QuadtreeObject* obj) {
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

        std::vector<QuadtreeObject*> findObjectsNearObject(sf::FloatRect queryObjectBounds, float distance) {
            sf::FloatRect queryBounds = queryObjectBounds;
            queryBounds.left -= distance;
            queryBounds.top -= distance;
            queryBounds.width += 2 * distance;
            queryBounds.height += 2 * distance;

            if (!boundary.intersects(queryBounds)) return std::vector<QuadtreeObject*>();

            std::vector<QuadtreeObject*> found;
            findObjectsInRange(queryBounds, found, distance, queryObjectBounds);

            return found;
        }

        void findObjectsInRange(const sf::FloatRect& range, std::vector<QuadtreeObject*>& found, float distance, sf::FloatRect queryObjectBounds) {
            // Check objects in the current node
            for (auto object : objects) {
                if (range.intersects(queryObjectBounds)) {
                    if (vm::distance(object->sprite.getPosition(), queryObjectBounds.getPosition()) <= distance) {
                        found.push_back(object);
                    }
                }
            }

            // Recur into children if the node is divided
            if (divided) {
                for (auto& child : children) {
                    if (child->boundary.intersects(range)) {
                        child->findObjectsInRange(range, found, distance, queryObjectBounds);
                    }
                }
            }
        }


        bool updateObjectPosition(QuadtreeObject* object, const int maxObjects) {
            // Check if the sprite is still within the boundary
            if (boundary.intersects(object->sprite.getGlobalBounds())) {
                // If already within the same node and correct position, no need to do anything
                return true;
            }
            else {
                // If the position is outside the current node's boundary, remove and reinsert the object
                return removeObject(object) && addObject(object, maxObjects);
            }
            return false;
        }

        // Method to draw the node boundaries for debugging
        void draw(GlobalValues* gv) {
            if (!divided) {
                gv->displayText("Num obj: " + std::to_string(objects.size()), sf::Vector2f(boundary.left + boundary.width / 2 - 10.f, boundary.top + boundary.height / 2 - 5.f), sf::Color::White);
            }

            if (divided) {
                for (auto& child : children) {
                    child->draw(gv);
                }
            }
                
            sf::RectangleShape rectangle(sf::Vector2f(boundary.width, boundary.height));
            rectangle.setPosition(boundary.left, boundary.top);
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineThickness(5.f);
            rectangle.setOutlineColor(sf::Color::Blue); // Node boundary

            gv->getWindow()->draw(rectangle);

            // Draw the objects' positions
            for (auto object : objects) {
                sf::CircleShape marker(10); // Small circle marker
                sf::Vector2f pos = object->sprite.getPosition();
                marker.setPosition(pos.x, pos.y);
                marker.setFillColor(sf::Color::Magenta); // Object position
                gv->getWindow()->draw(marker);
            }
        }
    };

    /// Quadtree class, this will be the main class that will be used to create the quadtree.
    template <typename T> requires HasGetSprite<T> && HasGetID<T>
    class Quadtree {
    public:
        const int maxObjects = 4;
        std::unique_ptr<Node> root;
        std::unordered_map<T*, QuadtreeObject*> objectMap;
        std::unordered_map<QuadtreeObject*, T*> reverseObjectMap;

        Quadtree(const sf::FloatRect initalBounds) {
            std::cout << "Initial boundary: " << initalBounds.left << ", " << initalBounds.top << ", " << initalBounds.width << ", " << initalBounds.height << std::endl;

            root = std::make_unique<Node>(initalBounds, maxObjects);
        }

        void update(sf::FloatRect& currentBoundary) {
            const float threshold = 0.1f;
            if (std::fabs(currentBoundary.left - root->boundary.left) > threshold ||
                std::fabs(currentBoundary.top - root->boundary.top) > threshold ||
                std::fabs(currentBoundary.width - root->boundary.width) > threshold ||
                std::fabs(currentBoundary.height - root->boundary.height) > threshold) {
                // The new boundary is different from the current root boundary
                extend(currentBoundary);
            }
            root->update(maxObjects);
        }

        bool addObject(T* object) {
            if (!root->boundary.contains(object->getSprite().getPosition())) {
                std::cerr << "Error: Object is out of quadtree bounds. Cannot add." << std::endl;
                return false;
            }

            auto qtObject = new QuadtreeObject(object->getID(), object->getSprite());
            if (root->addObject(qtObject, maxObjects)) {
                objectMap.insert({ object, qtObject });
                reverseObjectMap.insert({ qtObject, object });
                return true;
            }
            else {
                delete qtObject;  // Clean up if the addition failed
                std::cerr << "Error: Object addition failed." << std::endl;
                return false;
            }
        }


        bool removeObject(T* object) {
            auto it = objectMap.find(object);

            if (it == objectMap.end()) {
                std::cerr << "Error: Failed to remove object from quadtree." << std::endl;
                return false;
            }

            // Try to remove from quadtree
            if (root->removeObject(it->second)) {
                delete it->second;                  // Clean up the QuadtreeObject
                reverseObjectMap.erase(it->second);
                objectMap.erase(it);                // Remove from objectMap only after successful removal
                return true;
            }

            std::cerr << "Error: Object not found in objectMap." << std::endl;
            return false;
        }


        template <HasGetSprite U>
        std::vector<T*> findObjectsNearObject(U* queryObject, float distance) {
            auto nearby = root->findObjectsNearObject(queryObject->getSprite().getGlobalBounds(), distance);
            std::vector<T*> found;
            found.reserve(nearby.size());
            for (auto qtobject : nearby) {
                found.push_back(reverseObjectMap.at(qtobject));
			}
            return found;
        }

        // Extend function to enlarge the quadtree boundary
        void extend(const sf::FloatRect& newBoundary) {
            // Create a new root node with the expanded boundary
            auto newRoot = std::make_unique<Node>(newBoundary, maxObjects);

            // Add the current root node as a child to the new root
            newRoot->children.push_back(std::move(root));
            newRoot->divided = true;

            // Set the new root as the root of the quadtree
            root = std::move(newRoot);
        }

        void updateObjectPosition(T* object) {
            QuadtreeObject* qtObject = objectMap.at(object);
            if (!root->updateObjectPosition(qtObject, maxObjects)) {
                std::cerr << "Error: Object position update failed." << std::endl;
            }
        }

        void draw(GlobalValues* gv) {
            root->draw(gv);
        }

        std::vector<T*> getObjects() {
			std::vector<T*> objects;
			for (auto& obj : objectMap) {
				objects.push_back(obj.first);
			}
			return objects;
		}
    };
}
