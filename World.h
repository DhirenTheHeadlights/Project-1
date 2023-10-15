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
    void createWorld(sf::RenderWindow& window, sf::Event& event);
    void drawPellets(sf::RenderWindow& window, int numPellets);
    void removePelletWhenCollision(sf::RenderWindow& window);
    int getCircleSize() const;
    void drawInformation(sf::RenderWindow& window, std::string info, int textSize);
    void growCircle(float numCollisions);
private:
    std::vector<Pellet> activePellets;
    Hashmap hashmap;
    Map map;
    sf::View view;
    sf::Font font;
    sf::Clock mainTime;
    float addSize = 0;
    double collidedPellets_size = 0;
};
