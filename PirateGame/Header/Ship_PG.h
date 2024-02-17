#pragma once

/// <summary>
/// This class is used to represent the player ship in the game.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "GlobalValues_PG.h"
#include "ShipMovementHandler_PG.h"

namespace PirateGame {

	// This enum will be used to determine the ship type
	enum class ShipType { Player, Enemy };

	// This enum will be used to determine the ship class
	enum class ShipClass {
		Sloop,
		Brigantine,
		Frigate,
		ManOWar,
		Galleon
	};

	class Ship {
	public:
		Ship() : movementHandler(sprite) {};
		~Ship() {};

		// Create the ship and set its values
		void createShip(ShipType type, ShipClass shipClass);
		void move();
		void draw();

		// Setters
		void setHealth(float hp) { health = hp; }

		// Getters
		int getCollisionAxis() { return axis; }
		float getHealth() { return health; }
		sf::Sprite& getSprite() { return sprite; }
		ShipMovementHandler& getMovementHandler() { return movementHandler; }

	private:
		// Functions
		void setHealthBarPosition();

		// SFML Objects
		sf::RenderWindow* window;
		sf::Clock deltaTime;
		sf::Sprite sprite;
		sf::Texture texture;

		// Variables to store the ship's values
		float health = 1;
		float maxHealth = 1;
		float baseSpeed = 1;
		float speed = 1;
		float regenRate = 1;
		float scaling = 5;
		float rotation = 0;

		sf::Vector2f constSpriteBounds;
		sf::Vector2f spriteOrigin = sf::Vector2f(123.f, 128.f);

		ShipType shipType;
		ShipClass shipClass;

		// Collision rectangle
		sf::RectangleShape collisionRect;

		// Friction value and boolean and axis value
		int axis = 0;

		// Rectangle shape for the health bar
		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;

		// Clock for regenerating health
		sf::Clock healthRegenClock;

		// Movement handler
		ShipMovementHandler movementHandler;
	};

}
