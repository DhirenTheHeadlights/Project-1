#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

#include "Target.h"

class hashmap {
public:
	hashmap();
	void addTarget(Target* target);
	Target* getTargetAtCollision(sf::Vector2f position, float radius);
private:
	std::unordered_map<std::string, Target*> map;
	std::string generateKey(sf::Vector2f position);
	float distance(sf::Vector2f a, sf::Vector2f b);
	Target* getTargetInGrid(const std::string& key, sf::Vector2f position);
	int grid_size = 10;
};