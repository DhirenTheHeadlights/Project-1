#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "GameTemplate.h"
#include "CellGroup_AG.h"
#include "Map.h"
#include "CollisionManager_AG.h"
#include "PelletHandler_AG.h"
#include "Debugger_AG.h"
#include "InputHandler_AG.h"


namespace AgarGame {
    class World : public GameTemplate {
    public:
        World(sf::RenderWindow& window);
        void createWorld(sf::Event& event) override;
        void restart();
        void displayInfo();
        bool isGameOver() const;
    private:
        Map map;
        CollisionManager colMan;
        PelletHandler pelHan;
        Debugger debug;
        InputHandler input;
        sf::View view;
        sf::Font font;
        sf::RenderWindow& window;
        float addSize = 0;
        float startingCircleSize = 15.0f;
        float maxSize = 1000.0f;
        bool gameOver = false;
        CellGroup cellGroup = CellGroup(startingCircleSize, map);
    };
}