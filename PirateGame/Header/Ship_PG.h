#pragma once

/// Ship class is used to represent the ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unordered_map>

#include "GlobalValues_PG.h"
#include "GlobalTextureHandler_PG.h"
#include "GlobalChunkHandler_PG.h"
#include "GlobalIDManager_PG.h"

#include "ShipCannonHandler_PG.h"
#include "ShipMovementHandler_PG.h"
#include "ShipInputHandler_PG.h"
#include "ShipSailHandler_PG.h"
#include "ShipInventoryHandler_PG.h"
#include "ShipEnums_PG.h"

namespace PirateGame {
	class Ship {
	public:
		Ship() {};

		~Ship() {};

		// Create the ship and set its values. Random ship class if not specified.
		void setUpShip(ShipClass shipClass = ShipClass::Random);
		virtual void customShipSetUp() = 0; // Virtual method to allow for custom ship setup
		void update();
		virtual void customShipUpdate() = 0; // Virtual method to allow for custom ship update
		void draw();
		virtual void customShipDraw() = 0; // Virtual method to allow for custom ship draw

		// Setters
		void damageShip(float damagePerFrame) {
			health -= damagePerFrame;
			if (health < 0) health = 0;
		}
		void changeShipClass(ShipClass shipClass) {
			this->shipClass = shipClass;
			setUpShip(shipClass);
		}
		void setBaseSpeed(float speed) { shipProperties.baseSpeed = speed; }
		void setMaxHealth(float health) { shipProperties.maxHealth = health; }
		void setRegenRate(float rate) { shipProperties.regenRate = rate; }
		void setGroupID(int groupID) { this->groupID = groupID; }
		void setDead(bool isDead) { this->isDead = isDead; }

		// Getters
		float getHealth() const { return health; }
		sf::Sprite& getSprite() { return sprite; }
		int getID() const { return ID; }
		int getGroupID() const { return groupID; }
		bool getIsDead() const { return isDead; }

		ShipProperties& getSpecificShipProperties() { return shipProperties; }
		ShipClass getShipClass() const { return shipClass; }

		virtual ShipCannonHandler* getCannonHandler() { return SCH.get(); };
		virtual ShipSailHandler* getSailHandler() { return SSH.get(); };
		virtual ShipMovementHandler* getMovementHandler() { return SMH.get();}
		virtual ShipInputHandler* getInputHandler() { return SIH.get(); }
		virtual ShipInventoryHandler* getInventoryHandler() { return SIvH.get(); }

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

		// SFML Objects
		sf::Clock deltaTime;
		sf::Sprite sprite;

		// Variables to store the ship's values
		float health = 0;
		float scalingFactor = 4;
		bool isDead = false;

		sf::Vector2f constSpriteBounds;
		sf::Vector2f healthBarSize = { 100.f, 10.f };

		ShipClass shipClass = ShipClass::Sloop;
		ShipProperties shipProperties;

		// Clock for regenerating health
		sf::Clock healthRegenClock;
		sf::Time regenTime = sf::seconds(2.f);

		// Unique ID and group ID
		int ID = -1;
		int groupID = -1;
	protected:
		// Rectangle shape for the health bar
		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;

		// Handlers
		std::unique_ptr<ShipCannonHandler> SCH;
		std::unique_ptr<ShipSailHandler> SSH;
		std::unique_ptr<ShipInputHandler> SIH;
		std::unique_ptr<ShipInventoryHandler> SIvH;
		std::unique_ptr<ShipMovementHandler> SMH;
	};
}
