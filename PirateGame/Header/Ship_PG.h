#pragma once

/// Ship class is used to represent the ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unordered_map>

#include "GlobalValues_PG.h"
#include "ShipCannonHandler_PG.h"

namespace PirateGame {
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
		Ship() {
			SCH = std::make_unique<ShipCannonHandler>(sprite);
		};

		~Ship() {};

		// Create the ship and set its values. Random ship class if not specified.
		void setUpShip(ShipClass shipClass = ShipClass::Sloop);
		virtual void customShipSetUp() = 0; // Virtual method to allow for custom ship setup
		void update();
		virtual void customShipUpdate() = 0; // Virtual method to allow for custom ship update
		void draw();
		virtual void customShipDraw() = 0; // Virtual method to allow for custom ship draw

		// Get movement handler
		ShipCannonHandler& getCannonHandler() { return *SCH; }

		// Setters
		void damageShip(float damagePerSecond) {
			health -= damagePerSecond * deltaTime.restart().asSeconds();
			if (health < 0) health = 0;
		}
		void changeShipClass(ShipClass shipClass) {
			this->shipClass = shipClass;
			setUpShip(shipClass);
		}

		// Getters
		float getHealth() const { return health; }
		sf::Sprite& getSprite() { return sprite; }
		ShipProperties& getShipProperties() { return shipProperties; }
		ShipClass getShipClass() const { return shipClass; }

		std::string getShipClassString() {
			switch (shipClass) {
			case ShipClass::Sloop:
				return "Sloop";
			case ShipClass::Brigantine:
				return "Brigantine";
			case ShipClass::Frigate:
				return "Frigate";
			case ShipClass::ManOWar:
				return "Man O' War";
			case ShipClass::Galleon:
				return "Galleon";
			}
			return "Error: Ship class not found.";
		}

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
		float scalingFactor = 4;

		sf::Vector2f constSpriteBounds;

		ShipClass shipClass = ShipClass::Sloop;
		ShipProperties shipProperties;

		// Clock for regenerating health
		sf::Clock healthRegenClock;
		sf::Time regenTime = sf::seconds(2.f);

		// Handlers
		std::unique_ptr<ShipCannonHandler> SCH;
	protected:
		// Rectangle shape for the health bar
		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;
	};
}
