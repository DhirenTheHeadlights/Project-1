#pragma once
#include "Circle.h"
#include "Pellet.h"
#include "Map.h"
#include "Hashmap.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

class World {
public:
    World(Map& map, sf::RenderWindow& window);
    void createWorld(sf::RenderWindow& window, Map& map);
    void drawPellets(sf::RenderWindow& window, Map& map, Hashmap& hashmap);
    void removePelletWhenCollision(Circle& circle, Hashmap& hashmap, Map& map, sf::RenderWindow& window);
    int getCircleSize() const;
    void drawInformation(sf::RenderWindow& window);
    void growCircle(Map& map);
    void printActivePellets() const;
private:
    std::vector<Pellet> activePellets;
    Hashmap hashmap;
};
