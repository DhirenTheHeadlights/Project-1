#pragma once
#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Pellet.h"
#include "Circle.h"
#include "Globals.h"
#include "Hashmap.h"
const float MAX_ACCELERATION = 10.0f;
const float BASE_SPEED = 0.01f;

extern std::vector<sf::Color> colors;
extern Hashmap hashmap; // Added semicolon to resolve the error


class World {
public:
    World();
    void drawPellets(sf::RenderWindow& window);
private:
    std::vector<Pellet> pellets;
};