#pragma once
#include "Circle.h"
#pragma once

#include "Circle.h"
#include "Pellet.h"
#include "Map.h"
#include "Hashmap.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

class World {
public:
    World(sf::RenderWindow& window);
    void createWorld(sf::RenderWindow& window, sf::Event& event);
    void drawPellets(sf::RenderWindow& window, int numPellets);
    void removePelletWhenCollision(sf::RenderWindow& window);
    void drawInformation(sf::RenderWindow& window, const std::string& info, int textSize, float setPosX, float setPosY);
    void growCircle(float numCollisions);
    void restart();
    bool isGameOver() const;

private:
    std::vector<std::shared_ptr<Pellet>> activePellets;
    Hashmap hashmap;
    Map map;
    sf::View view;
    sf::Font font;
    sf::Clock mainTime;
    float addSize = 0;
    float zoomMultiplier = 1.0f;
    double collidedPelletsSize = 0;
    bool gameOver = false;
    float startingCircleSize = 15.0f;
    Circle circle = Circle(startingCircleSize);
};