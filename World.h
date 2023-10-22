#pragma once

#include "CellGroup.h"
#include "Pellet.h"
#include "Map.h"
#include "Hashmap.h"
#include "CollisionManager.h"
#include "PelletHandler.h"
//#include "Debugger.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

class World {
public:
    World(sf::RenderWindow& window);
    void createWorld(sf::RenderWindow& window, sf::Event& event);
    void restart();
    bool isGameOver() const;
    void drawInformation(sf::RenderWindow& window, const std::string& info, int textSize, float setPosX, float setPosY);
    void drawVector(const sf::Vector2f& start, const sf::Vector2f& direction, sf::RenderWindow& window, float magnitude, sf::Color color);
private:
    Map map;
    CollisionManager colMan;
    PelletHandler pelletHandler;
    CellGroup cellGroup = CellGroup(startingCircleSize, map);
    //Debugger debug;
    sf::View view;
    sf::Font font;
    float zoomMultiplier = 1.0f;
    float startingCircleSize = 15.0f;
    float avgX = 0.0f, avgY = 0.0f;
    bool gameOver = false;
};