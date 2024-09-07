#pragma once

/// This class is used to represent the player ship in the game.

#include <cmath>

#include "GlobalContext_PG.h"

#include "Ship_PG.h"
#include "PlayerShipInputHandler_PG.h"
#include "PlayerShipMovementHandler_PG.h"

namespace PirateGame {
	class PlayerShip : public Ship {
	public:
		PlayerShip(GlobalContext& context) : Ship(context) {};

		// Experience
		void addExperience(const float exp) {
			experience += exp;
		}
		float getExp() const { return experience; }
		float getExpToLevelUp() const { return experienceToLevelUp; }
		int getPlayerLevel() const { return playerLevel; }

		// Create the ship and set its values
		virtual void customShipSetUp() override;
		virtual void customShipUpdate() override;
		virtual void customShipDraw() override;

		// Overridden getters
		PlayerShipInputHandler* getInputHandler() override { return dynamic_cast<PlayerShipInputHandler*>(SIH.get()); }
		PlayerShipMovementHandler* getMovementHandler() override { return dynamic_cast<PlayerShipMovementHandler*>(SMH.get()); }
	private:
		// Experience
		float experience = context.JSL->getGameData().saveData.playerExperience;
		float experienceToLevelUp = context.JSL->getGameData().saveData.playerExperienceToLevelUp;
		int playerLevel = context.JSL->getGameData().saveData.playerLevel;
	};
}
