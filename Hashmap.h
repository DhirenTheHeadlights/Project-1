#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Map.h"
#include "Pellet.h"
#include "Circle.h"
#include <variant>

class Pellet;
class Circle;

struct GridObject {
    std::variant<Pellet, Circle> object; 
};

class Hashmap {
public:
    Hashmap(Map& map, sf::RenderWindow& window);
    void assignPellet(const Pellet& pellet, Map& map);
    void assignCircle(const Circle& circle, Map& map);
    void checkCollision(Circle& circle, Hashmap& hashmap, Map& map, sf::RenderWindow& window);
    std::vector<Pellet> getPelletsInSameCell(float x, float y, Map& map) const;
private:
    std::unordered_map<std::string, std::vector<GridObject>> hashmap;
    std::string generateKey(int x, int y) const;
};