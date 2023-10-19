#include "Pellet.h"
#include "Circle.h"
#include "Map.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <set>

class Hashmap {
public:
    Hashmap(sf::RenderWindow& window);
    void assignPellet(Pellet& pellet, Map& map);
    std::set<Pellet*> checkCollisionWithinBoundsOfCircle(Circle* circle, Map& map);
private:
    std::unordered_map<std::string, std::set<Pellet*>> hashmap;
    std::string generateKey(int x, int y) const;
    std::set<Pellet*> getPelletsInSameCell(float x, float y) const;
    void checkCollision(const std::string& key, Circle* circle, std::set<Pellet*>& collidedPellets);
    void drawCell(int x, int y, Map& map, sf::Color color);
    sf::RenderWindow& window;
    float minDistForCollision = 0.5f;
};