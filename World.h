#pragma once
#include "CellGroup.h"
#include "Map.h"
#include "CollisionManager.h"
#include "PelletHandler.h"
#include "Debugger.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class World {
public:
    World(sf::RenderWindow& window);
    void createWorld(sf::RenderWindow& window, sf::Event& event);
    void restart();
    bool isGameOver() const;
private:
    Map map;
    CollisionManager colMan;
    PelletHandler pelHan;
    Debugger debug;
    sf::View view;
    sf::Font font;
    float addSize = 0;
    float zoomMultiplier = 1.0f;
    float startingCircleSize = 15.0f;
    float maxSize = 1000.0f;
    float avgX = 0.0f, avgY = 0.0f;
    bool gameOver = false;
    CellGroup cellGroup = CellGroup(startingCircleSize, map);
};