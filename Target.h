#pragma once
#include <SFML/Graphics.hpp>

class Target {
public:
	Target(sf::Vector2f position, float radius);
	void setPos(sf::Vector2f position);
	sf::Vector2f getPos();
	sf::CircleShape getShape();
	bool isActive();
	void deActivate();
private:
	sf::CircleShape shape;
	sf::Vector2f position;
	bool activeStatus = true;
};