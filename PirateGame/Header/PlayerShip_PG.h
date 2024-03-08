#pragma once

/// This class is used to represent the player ship in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unordered_map>

#include "GlobalValues_PG.h"
#include "Ship_PG.h"
#include "PlayerShipInputHandler_PG.h"
#include "PlayerShipMovementHandler_PG.h"
#include "ShipInventoryHandler_PG.h"

namespace PirateGame {
	class PlayerShip : public Ship {
	public:
		PlayerShip() {
			SIH = std::make_unique<PlayerShipInputHandler>(getSprite());
			SMH = std::make_unique<PlayerShipMovementHandler>(getSprite());
			SIvH = std::make_unique<ShipInventoryHandler>();
		};

		~PlayerShip() {};

		// Create the ship and set its values
		virtual void customShipSetUp() override;
		virtual void customShipUpdate() override;
		virtual void customShipDraw() override;

		// Get movement handler
		PlayerShipInputHandler& getInputHandler() { return *SIH; }
		ShipInventoryHandler& getInventoryHandler() { return *SIvH; }
		PlayerShipMovementHandler& getMovementHandler() { return *SMH; }

	private:
		// Handlers
		std::unique_ptr<PlayerShipInputHandler> SIH;
		std::unique_ptr<ShipInventoryHandler> SIvH;
		std::unique_ptr<PlayerShipMovementHandler> SMH;
	};
}
