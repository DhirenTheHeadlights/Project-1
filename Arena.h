#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

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

	Target* createTarget();
	void updateTargets();
	void checkCollision();
	void drawTarget();
	void assignTargetsToHashmap();

	hashmap hashmap;
};
