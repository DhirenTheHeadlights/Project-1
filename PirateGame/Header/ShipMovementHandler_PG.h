#pragma once

/// <summary>
/// This class controls the movement and collisions of the ship class
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>

class ShipMovementHandler_PG {
	public:
	ShipMovementHandler_PG() {};
	~ShipMovementHandler_PG() {};

	// Movement functions
	void move(sf::Vector2f map);
	void direction(sf::Vector2f veclocity, float elapsed, sf::Vector2f map);
	void collisionMovement(sf::Vector2f normalVector);
	void stop();
private:
	// SFML Objects
	sf::RenderWindow* window = nullptr;
	sf::Clock deltaTime;
};

