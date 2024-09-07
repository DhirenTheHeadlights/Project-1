#pragma once

/// This is a template class for a hashmap that will store pointers to objects.
/// Each object needs to have a sprite to grab a position from.

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <SFML/Graphics.hpp>

#include "GlobalIDManager_PG.h"
#include "GlobalValues_PG.h"
#include "VectorMath.h"

namespace PirateGame {
    /// Concept to check if a class has a method getID that returns an ID*
    template <class T>
    concept HasGetID = requires(T t) {
        { t.getID() } -> std::convertible_to<ID*>;
    };

    /// Concept to check if a class has a method getSprite that returns an sf::Sprite
    template <class T>
    concept HasGetSprite = requires(T t) {
        { t.getSprite() } -> std::convertible_to<sf::Sprite>;
    };

    /// Concept to check if a class has a public member variable named id
    template <class T>
    concept HasPublicID = requires(T t) {
        { t.id } -> std::convertible_to<ID*>;
    };

    /// Concept to check if a class has a public member variable named sprite
    template <class T>
    concept HasPublicSprite = requires(T t) {
        { t.sprite } -> std::convertible_to<sf::Sprite>;
    };

    /// Concept to check if a class has either getID() or a public member variable id
    template <class T>
    concept SupportsID = HasGetID<T> || HasPublicID<T>;

    /// Concept to check if a class has either getSprite() or a public member variable sprite
    template <class T>
    concept SupportsSprite = HasGetSprite<T> || HasPublicSprite<T>;

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
                for (const auto& child : children) {
                    child->update(maxObjects);
                }
            }

            if (!divided) return;

            int totalObjects = 0;
            for (const auto& child : children) {
                totalObjects += static_cast<int>(child->objects.size());
            }

            // If total objects in children are less than maxObjects and no child is divided, merge them
            if (totalObjects < maxObjects && std::ranges::none_of(children, [](const auto& child) { return child->divided; })) {
                merge();
            }
        }

        void subdivide(const int maxObjects) {
	        const float x = boundary.left;
	        const float y = boundary.top;
	        const float width = boundary.width / 2;
	        const float height = boundary.height / 2;

            children.push_back(std::make_unique<Node>(sf::FloatRect(x, y, width, height), maxObjects));
            children.push_back(std::make_unique<Node>(sf::FloatRect(x + width, y, width, height), maxObjects));
            children.push_back(std::make_unique<Node>(sf::FloatRect(x, y + height, width, height), maxObjects));
            children.push_back(std::make_unique<Node>(sf::FloatRect(x + width, y + height, width, height), maxObjects));

            divided = true;

            // Redistribute objects to new children
            for (auto obj : objects) {
                for (const auto& child : children) {
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
                return std::ranges::any_of(children, [&](const auto& child) {
					return child->addObject(qtObject, maxObjects);
				});
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
            const auto it = std::ranges::find_if(objects, [&](const QuadtreeObject* obj) {
                return obj == qtObject;
                });

            if (it != objects.end()) {
                objects.erase(it);
                qtObject->unregisterNode(this); // Update the node registration
                return true;
            }

            // Check children if not found
            if (divided) {
                for (const auto& child : children) {
                    if (child->removeObject(qtObject)) {
                        return true;
                    }
                }
            }
            return false;
        }

        void merge() {
            if (divided) {
                for (const auto& child : children) {
                    child->merge();
                    objects.insert(objects.end(), child->objects.begin(), child->objects.end());
                }
                children.clear();
                divided = false;
            }
        }

        std::vector<QuadtreeObject*>& findObjectsNearObject(const sf::FloatRect& queryObjectBounds, const float distance, const int numObjectsInQuadtree) const {
            sf::FloatRect queryBounds = queryObjectBounds;
            queryBounds.left -= distance;
            queryBounds.top -= distance;
            queryBounds.width += 2 * distance;
            queryBounds.height += 2 * distance;

            if (!boundary.intersects(queryBounds)) {
                std::vector<QuadtreeObject*> empty;
				return empty;
            }

            std::vector<QuadtreeObject*> found;
            findObjectsInRange(queryBounds, found, distance, queryObjectBounds);

            return found;
        }

        void findObjectsInRange(const sf::FloatRect& range, std::vector<QuadtreeObject*>& found, float distance, const sf::FloatRect& queryObjectBounds) const {
            found.reserve(objects.size()); // Reserve space with the worst case scenario

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
    };

    /// Quadtree class, this will be the main class that will be used to create the quadtree.
    template <typename T> requires SupportsSprite<T> && SupportsID<T>
    class Quadtree {
    public:
        template <typename V> requires SupportsSprite<V>
        static sf::Sprite& getObjectSprite(V* object) {
            if constexpr (HasGetSprite<V>) {
				return object->getSprite();
			}
			else {
				return object->sprite;
			}
        }

        template <typename V> requires SupportsID<V>
        static ID* getObjectID(V* object) {
            if constexpr (HasGetID<V>) {
                return object->getID();
            }
            else {
                return object->id;
            }
        }

        const int maxObjects = 4;
        std::unique_ptr<Node> root;
        std::unordered_map<T*, QuadtreeObject*> objectMap;
        std::unordered_map<QuadtreeObject*, T*> reverseObjectMap;

        explicit Quadtree(const sf::FloatRect initialBounds) {
            std::cout << "Initial boundary: " << initialBounds.left << ", " << initialBounds.top << ", " << initialBounds.width << ", " << initialBounds.height << std::endl;

            root = std::make_unique<Node>(initialBounds, maxObjects);
        }

        void update(const sf::FloatRect& currentBoundary) {
	        constexpr float threshold = 0.1f;
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
            if (!root->boundary.contains(getObjectSprite(object).getPosition())) {
                std::cerr << "Error: Object is out of quadtree bounds. Cannot add." << std::endl;
                return false;
            }

            auto qtObject = new QuadtreeObject(getObjectID(object), getObjectSprite(object));
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


        template <SupportsSprite U>
        std::vector<T*> findObjectsNearObject(U* queryObject, float distance) {
            auto& nearby = root->findObjectsNearObject(getObjectSprite(queryObject).getGlobalBounds(), distance, objectMap.size());
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

        void draw(GlobalValues* gv) const {
            drawNode(root.get(), gv);
        }

        static void drawNode(Node* node, GlobalValues* gv) {
			if (!node->divided) {
				gv->displayText("Num obj: " + std::to_string(node->objects.size()), sf::Vector2f(node->boundary.left + node->boundary.width / 2 - 10.f, node->boundary.top + node->boundary.height / 2 - 5.f), sf::Color::White);
			}

			if (node->divided) {
				for (auto& child : node->children) {
					drawNode(child.get(), gv);
				}
			}

			sf::RectangleShape rectangle(sf::Vector2f(node->boundary.width, node->boundary.height));
			rectangle.setPosition(node->boundary.left, node->boundary.top);
			rectangle.setFillColor(sf::Color::Transparent);
			rectangle.setOutlineThickness(5.f);
			rectangle.setOutlineColor(sf::Color::Blue); // Node boundary

			gv->getWindow()->draw(rectangle);

			// Draw the objects' positions
			for (auto object : node->objects) {
				sf::CircleShape marker(10); // Small circle marker
				sf::Vector2f pos = object->sprite.getPosition();
				marker.setPosition(pos.x, pos.y);
				marker.setFillColor(sf::Color::Magenta); // Object position
				gv->getWindow()->draw(marker);
			}
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
