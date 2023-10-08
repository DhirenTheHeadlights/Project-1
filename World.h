#pragma once
#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Pellet.h"
#include "Circle.h"
#include "Hashmap.h"

const float MAX_ACCELERATION = 10.0f;
const float BASE_SPEED = 0.01f;

class Hashmap;
class Circle;

class World {
public:
    World(Map& map, sf::RenderWindow& window);
    void drawCircle(sf::RenderWindow& window, Hashmap& hashmap, Map& map);
    void drawPellets(sf::RenderWindow& window, Map& map, Hashmap& hashmap);
private:
    std::vector<Pellet> pellets;
};