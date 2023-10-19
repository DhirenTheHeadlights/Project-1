#pragma once

#include "CellGroup.h"
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
    void checkCollisionForEveryCell();
    void removePelletWhenCollision(Circle* cell);
    void drawInformation(sf::RenderWindow& window, const std::string& info, int textSize, float setPosX, float setPosY);
    void restart();
    void drawVector(const sf::Vector2f& start, const sf::Vector2f& direction, sf::RenderWindow& window, float magnitude, sf::Color color);
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
    float startingCircleSize = 15.0f;
    float maxSize = 1000.0f;
    float timeBetweenPelletsSpawning = 0.1f;
    bool gameOver = false;
    CellGroup cellGroup = CellGroup(startingCircleSize, map);
};