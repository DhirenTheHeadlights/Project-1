#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

#include "Target_AT.h"

class Hashmap_AT {
public:
	Hashmap_AT();
	void addTarget(Target* target);
	void clear();
	Target* getTargetAtCollision(sf::Vector2f position, float radius);
private:
	std::unordered_map<std::string, Target*> map;
	std::string generateKey(sf::Vector2f position);
	float distance(sf::Vector2f a, sf::Vector2f b);
	Target* getTargetInGrid(const std::string& key, sf::Vector2f position);
	int grid_size = 10;
};