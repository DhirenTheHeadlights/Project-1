#pragma once
#include <SFML/Graphics.hpp>
#include "Hashmap.h"
#include "Debug.h"
#include "Map.h"
#include "CellGroup.h"

class CollisionHandler
{
public:
	CollisionHandler(sf::RenderWindow& window, Map& map);
	std::set<Pellet*> pelletCollisionWithinBoundsOfCircle(Circle* circle, Map& map, Debug& debug);
	void checkCollisionWithPellet(const std::string& key, Circle* circle, std::set<Pellet*>& collidedPellets);
	void checkCollisionForEveryCell(Map& map, CellGroup& cells, Debug& debug);
	void separateCells(Circle* cell, Map& map);
	void removePelletWhenCollision(Circle* cell, Map& map, Debug& debug);
	void assignPelletToHM(std::vector<std::shared_ptr<Pellet>> pellets, Map& map);
	void assignCellToHM(Circle& cell, Map& map);
private:
	sf::RenderWindow& window;
	Hashmap hashmap;
	float minDistForCollision = 0.5f;
	std::set<Pellet*> toBeRemoved;
};
