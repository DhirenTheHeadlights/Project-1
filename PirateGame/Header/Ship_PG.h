#pragma once

/// Ship class is used to represent the ships in the game.

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "GlobalContext_PG.h"

#include "ShipCannonHandler_PG.h"
#include "ShipMovementHandler_PG.h"
#include "ShipInputHandler_PG.h"
#include "ShipSailHandler_PG.h"
#include "ShipInventoryHandler_PG.h"
#include "ShipEnums_PG.h"
#include "Region_PG.h"

namespace PirateGame {
	class Ship {
	public:
		Ship(GlobalContext& context) : context(context), id(context.GIDM->generateID()) {};

		// Create the ship and set its values. Random ship class if not specified.
		void setUpShip(const ShipClass shipClass = ShipClass::Random, const Region region = Region::SandyShores);
		virtual void customShipSetUp() = 0; // Virtual method to allow for custom ship setup
		void update();
		virtual void customShipUpdate() = 0; // Virtual method to allow for custom ship update
		void draw();
		virtual void customShipDraw() = 0; // Virtual method to allow for custom ship draw

		// Setters
		void damageShip(const float damagePerFrame) {
			health -= (health < 0.f) ? 0.f : damagePerFrame;
		}
		void changeShipClass(ShipClass shipClass) {
			this->shipClass = shipClass;
			setUpShip(shipClass);
		}
		void setGroupID(ID* groupID) { this->groupID = groupID; }
		void setDead(bool isDead) { this->isDead = isDead; }
		void setHealth(float health) { this->health = health; }

		// Getters
		float getHealth() const { return health; }
		sf::Sprite& getSprite() { return sprite; }
		ID* getID() const { return id.get(); }
		ID* getGroupID() const { return groupID; }
		bool getIsDead() const { return isDead; }
		Region getBirthRegion() const { return birthRegion; }

		ShipProperties& getSpecificShipProperties() { return shipProperties; }
		ShipClass getShipClass() const { return shipClass; }

		virtual ShipCannonHandler* getCannonHandler() { return SCH.get(); };
		virtual ShipSailHandler* getSailHandler() { return SSH.get(); };
		virtual ShipMovementHandler* getMovementHandler() { return SMH.get();}
		virtual ShipInputHandler* getInputHandler() { return SIH.get(); }
		virtual ShipInventoryHandler* getInventoryHandler() { return SIvH.get(); }

		std::string getShipClassString() const {
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
			default: ;
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

		ShipClass shipClass = ShipClass::Sloop;
		ShipProperties shipProperties;
		Region birthRegion = Region::SandyShores;

		// Clock for regenerating health
		sf::Clock healthRegenClock;
		sf::Time regenTime = sf::seconds(2.f);

		// Unique ID and group ID
		std::shared_ptr<ID> id;
		ID* groupID = nullptr;
	protected:
		// Context
		GlobalContext& context;

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
