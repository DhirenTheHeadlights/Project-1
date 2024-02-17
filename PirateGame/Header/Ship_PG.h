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
		Ship() : SMH(sprite) {};
		~Ship() {};

		// Create the ship
		void createShip(ShipType type, ShipClass shipClass);

		// Get movement handler
		ShipMovementHandler& getMovementHandler() { return SMH; }

		// Draw functions
		void draw(sf::Vector2f map);
		void drawVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color = sf::Color::Red);

		// Setters
		void setHealth(float hp) { health = hp; }

		// Getters
		sf::Vector2f getSpritePosition() { return sprite.getPosition(); }
		float getHealth() { return health; }
		sf::Sprite& getSprite() { return sprite; }
	private:
		// SFML Objects
		sf::RenderWindow* window;
		sf::Clock deltaTime;
		sf::Sprite sprite;
		sf::Texture texture;

		// Variables to store the ship's values
		float health = 1;
		float maxHealth = 1;
		float baseSpeed = 1;
		float regenRate = 1;
		float scaling = 5;
		float rotation = 0;

		sf::Vector2f constSpriteBounds;
		sf::Vector2f spriteOrigin = sf::Vector2f(123.f, 128.f);

		ShipType shipType;
		ShipClass shipClass;

		// Collision rectangle
		sf::RectangleShape collisionRect;

		// Rectangle shape for the health bar
		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;

		// Clock for regenerating health
		sf::Clock healthRegenClock;

		// Movement Handler
		ShipMovementHandler SMH;
	};

}
