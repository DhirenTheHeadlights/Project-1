#pragma once

/// <summary>
/// This class is used to represent the player ship in the game.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unordered_map>

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

	// Struct to hold ship properties
	struct ShipProperties {
		float baseSpeed;
		float health;
		float regenRate;
		std::string texturePath;
		float scaleX, scaleY;
		sf::Vector2f origin = sf::Vector2f(126.f, 128.f);
	};

	class Ship {
	public:
		Ship() : SMH(sprite) {};
		~Ship() {};

		// Create the ship and set its values
		void createShip(ShipType type, ShipClass shipClass);
		void updateAndDraw();

		// Get movement handler
		ShipMovementHandler& getMovementHandler() { return SMH; }

		// Setters
		void damageShip(float damagePerSecond) {
			health -= damagePerSecond * deltaTime.restart().asSeconds();
			if (health < 0) health = 0;
		}

		// Getters
		float getHealth() { return health; }
		sf::Sprite& getSprite() { return sprite; }

	private:
		// Static map for ship configurations
		static std::unordered_map<ShipClass, ShipProperties> shipConfig;

		// Functions
		void setHealthBarPosition();
		void regenerateHealth();
		sf::VertexArray createVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color = sf::Color::Red);

		// SFML Objects
		sf::RenderWindow* window = nullptr;
		sf::Clock deltaTime;
		sf::Sprite sprite;
		sf::Texture texture;

		// Variables to store the ship's values
		float health = 1;
		float maxHealth = 1;
		float baseSpeed = 1;
		float regenRate = 1;
		float scalingFactor = 5;
		float rotation = 0;

		sf::Vector2f constSpriteBounds;

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
