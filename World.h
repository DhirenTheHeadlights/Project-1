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
    World(sf::RenderWindow& window);
    void createWorld(sf::RenderWindow& window);
    void drawPellets(sf::RenderWindow& window);
    void removePelletWhenCollision(sf::RenderWindow& window);
    int getCircleSize() const;
    void drawInformation(sf::RenderWindow& window, std::string info);
    void growCircle(int numCollisions);
private:
    std::vector<Pellet> activePellets;
    Hashmap hashmap;
    Map map;
};
