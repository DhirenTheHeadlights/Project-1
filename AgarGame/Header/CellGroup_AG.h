#pragma once
#include <SFML/Graphics.hpp>
#include "Circle_AG.h"
#include <vector>
#include <algorithm>

class CellGroup {
public:
	CellGroup(const float startSize, Map& map);
	~CellGroup();
	void split();
	void removeCircle(Circle& cell, Map& map);
	void draw(sf::RenderWindow& window) const;
	void move(double moveSpeed, Map& map, sf::RenderWindow& window);
	void reset();
	std::vector<Circle*> getCellGroup();
	const float getMinSize() const;
	const sf::Vector2f getAvgPos() const;
private:
	float x, y;
	float cellSize = 0.0f;
	float minSize = 16.0f;
	std::vector<Circle*> cells;
	int numSplits = 1;
	int numCells = 1;
};