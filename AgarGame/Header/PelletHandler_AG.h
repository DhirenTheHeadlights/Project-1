#pragma once
#include <SFML/Graphics.hpp>
#include "Pellet_AG.h"
#include "Map.h"

class PelletHandler {
public:
	PelletHandler(sf::RenderWindow& window);
	void drawPellets(sf::RenderWindow& window, Map& map, const int numPellets);
	void eraseIfInactive();
	std::vector<std::shared_ptr<Pellet>> getActivePellets();
private:
	sf::Clock mainTime;
	float timeBetweenPelletsSpawning = 0.1f;
	std::vector<std::shared_ptr<Pellet>> activePellets;
};