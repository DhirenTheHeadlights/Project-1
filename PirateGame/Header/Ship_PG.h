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
#include "ShipInputHandler_PG.h"

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
		float baseSpeed = 1.f;
		float maxHealth = 100.f;
		float regenRate = 0.5f;
		std::string texturePath = "";
		float scaleX = 1.f, scaleY = 1.f;
		int numCannons = 1;
	};

	class Ship {
	public:
		Ship() : SIH(sprite) {};
		~Ship() {};

		// Create the ship and set its values
		void createShip(ShipType type, ShipClass shipClass);
		void updateAndDraw();
		

		// Get movement handler
		ShipMovementHandler& getMovementHandler() { return SIH.getMovementHandler(); }

		// Setters
		void damageShip(float damagePerSecond) {
			health -= damagePerSecond * deltaTime.restart().asSeconds();
			if (health < 0) health = 0;
		}
		void addItemToInventory(std::string item, int quantity) { inventory.push_back(std::make_pair(item, quantity)); }

		// Getters
		float getHealth() const { return health; }
		sf::Sprite& getSprite() { return sprite; }
		ShipProperties& getShipProperties() { return shipProperties; }



	private:
		// Static map for ship configurations
		static std::unordered_map<ShipClass, ShipProperties> ShipConfig;

		// Helper methods
		void setHealthBarPosition();
		void regenerateHealth();
		sf::VertexArray createVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color = sf::Color::Red);

		// SFML Objects
		sf::Clock deltaTime;
		sf::Sprite sprite;
		sf::Texture texture;

		// Variables to store the ship's values
		float health = 0;
		float scalingFactor = 5;

		sf::Vector2f constSpriteBounds;

		ShipType shipType = ShipType::Player;
		ShipClass shipClass = ShipClass::Sloop;
		ShipProperties shipProperties;

		// Rectangle shape for the health bar
		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;

		// Clock for regenerating health
		sf::Clock healthRegenClock;

		// Handlers
		ShipInputHandler SIH;

		std::vector<std::pair<std::string, int>> inventory;
	};

}
