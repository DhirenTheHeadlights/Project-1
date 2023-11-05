#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Target.h"
#include "AimTrainerHashmap.h"

class Arena {
public:
	Arena(sf::RenderWindow& window);
	void createArena();
private:
	sf::RenderWindow& window;
	std::vector<Target*> activeTargets;
	sf::VideoMode desktop;
	float size = 10;
	float maxTargets = 10;

	void drawTargets();
	void updateTargets();
	void checkCollision();

	hashmap hashmap;
};
