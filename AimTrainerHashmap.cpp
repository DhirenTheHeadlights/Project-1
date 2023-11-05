#include "AimTrainerHashmap.h"

hashmap::hashmap() {
}

void hashmap::addTarget(Target* target) {
	std::string key = generateKey(target->getPos());
	if (target->isActive()) map[key] = target;
	else map.erase(key);
}

std::string hashmap::generateKey(sf::Vector2f position) {
	return std::to_string(position.x) + "," + std::to_string(position.y);
}

Target* hashmap::getTargetAtCollision(sf::Vector2f position) {
	std::string key = generateKey(position);
	if (map.count(key)) {
		return map.at(key);
	}
	else {
		return nullptr;
	}
}
