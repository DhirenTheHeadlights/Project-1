#pragma once
#include <SFML/Graphics.hpp>
#include "Pellet.h"
#include "Map.h"
#include <vector>
#include <set>

class PelletManager {
public:
	PelletManager(sf::RenderWindow& window);
	void drawPellets(sf::RenderWindow& window, Map& map, const int numPellets);
	void eraseIfInactive();
	std::vector<std::shared_ptr<Pellet>> getActivePellets();
private:
	sf::RenderWindow& window;
	sf::Clock mainTime;
	float timeBetweenPelletsSpawning = 0.1f;
	std::vector<std::shared_ptr<Pellet>> activePellets;
	bool gameOver = false;
};