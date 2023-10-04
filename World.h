#pragma once
#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Pellet.h"
#include "Circle.h"
#include "Globals.h"
#include "Hashmap.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const float MAX_ACCELERATION = 10.0f;
const float BASE_SPEED = 0.01f;

extern std::vector<sf::Color> colors;

class World {
public:
    void drawPellets(sf::RenderWindow& window, Hashmap hashmap);
    void RemovePelletWhenCollision(Pellet pellet, Circle& circle, Hashmap& hashmap);
private:
    std::vector<Pellet> pellets;
};