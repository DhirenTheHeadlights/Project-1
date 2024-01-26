#pragma once

/// <summary>
/// This class is used to represent the world in the game.
/// </summary>

#include <SFML/Graphics.hpp>

#include "Player_PG.h"
#include "View_PG.h"
#include "Ship_PG.h"
#include "LandMassHandler_PG.h"
#include "GameStateManager_PG.h"
#include "Map.h"

class World_PG {
public:
	World_PG(sf::RenderWindow& window);
	~World_PG();

	void createWorld();
	void gameLoop();

private:
	// Variables to store the world's values
	int width = 10000;
	int height = 10000;
	int cellSize = 10;

	// Map size
	sf::Vector2f worldMap = sf::Vector2f(width, height);

	// Main clock
	sf::Clock mainClock;

	// This will be implemented later
	Player player;

	// This will be implemented later
	Ship ship;

	// This will be implemented later
	View view;

	// LandMass Handler
	LandMassHandler LMHandler;

	// GameState Manager
	GameStateManager_PG GSM;

	// This will be implemented later
	sf::RenderWindow& window;

	// Map
	Map map;
};

