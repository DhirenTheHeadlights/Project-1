#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Target {
public:
	Target(sf::Vector2f position, float radius);
	~Target();
	void setPos(sf::Vector2f position);
	sf::Vector2f getPos() const;
	sf::CircleShape getShape();
	bool isActive();
	void deActivate();
private:
	sf::CircleShape shape;
	sf::Vector2f position;
	bool activeStatus = true;
};