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
	Target* getTargetAtCollision(sf::Vector2f position);
private:
	std::unordered_map<std::string, Target*> map;
	std::string generateKey(sf::Vector2f position);
};