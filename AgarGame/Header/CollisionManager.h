#pragma once
#include "Pellet.h"
#include "Circle.h"
#include "Map.h"
#include "CellGroup.h"
#include "Hashmap.h"
#include "Debugger.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <iostream>


class CollisionManager {
public:
	CollisionManager(sf::RenderWindow& window, Map& map);
	std::set<Pellet*> pelletCollisionWithinBoundsOfCircle(sf::RenderWindow& window, Circle* circle, Map& map);
	void checkCollisionWithPellet(const std::string& key, Circle* circle, std::set<Pellet*>& collidedPellets);
	void separateAllCells(CellGroup& cells, Map& map);
	void removePelletWhenCollision(sf::RenderWindow& window, Circle* cell, Map& map, std::vector<std::shared_ptr<Pellet>> activePellets);
	void checkPelletCollisionForEveryCell(sf::RenderWindow& window, CellGroup& cells, Map& map, std::vector<std::shared_ptr<Pellet>> activePellets);
	void assignCells(CellGroup& cells, Map& map);
	void assignPellets(std::vector<std::shared_ptr<Pellet>> activePellets, Map& map);
	void moveAndSeparateCells(CellGroup& cells, Map& map, sf::RenderWindow& window, float moveSpeed);
private:
	void separateCells(Circle* cell, Map& map);
	float minDistForCollision = 0.5f;
	sf::Clock separateClock;
	Hashmap hashmap;
	Debugger debugger;
};