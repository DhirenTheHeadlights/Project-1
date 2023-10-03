#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>


class Hashmap {
public:
	Hashmap();
	void assignHash(float x, float y);
	void checkCollision(float x1, float y1, float x2, float y2);
private:
	std::unordered_map<int, int> Map;
	int XorderedMap;
	int YorderedMap;
};