#pragma once
#include <SFML/Graphics.hpp>
#include "Circle.h"
#include <vector>
#include <algorithm>

class CellGroup {
public:
	CellGroup(const float startSize, Map& map);
	~CellGroup();
	void split();
	void removeCircle(Circle& cell, Map& map);
	void draw(sf::RenderWindow& window);
	void move(double moveSpeed, Map& map, sf::RenderWindow& window);
	void reset();
	std::vector<Circle*> getCellGroup();
private:
	float x, y;
	float cellSize = 0.0f;
	float minSize = 32.0f;
	std::vector<Circle*> cells;
	int numSplits = 1;
	int numCells = 1;
	bool collision();
};