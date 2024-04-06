#pragma once

/// This class is almost identical to PlayerShip, but it is used for the enemy ships.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unordered_map>

#include "GlobalValues_PG.h"
#include "Ship_PG.h"
#include "EnemyShipInputHandler_PG.h"
#include "EnemyShipMovementHandler_PG.h"
#include "HashmapTemplate_PG.h"
#include "GlobalIDManager_PG.h"

namespace PirateGame {
	class EnemyShip : public Ship {
	public:
		EnemyShip() {};
		~EnemyShip() {};

		// Custom functions
		void customShipSetUp() override;
		void customShipUpdate() override;
		void customShipDraw() override;

		// Getters
		EnemyShipMovementHandler& getMovementHandler() { return *SMH; }
		EnemyShipInputHandler& getInputHandler() { return *SIH; }
		int getGroupID() { return groupID; }

		// Setters
		void setTargetPosition(sf::Vector2f targetPos) {
			this->targetPosition = targetPos;
			SIH->setTargetPos(targetPos);
			SMH->setTargetPosition(targetPos);
		}
		void setGroupID(int groupID) { this->groupID = groupID; }

	private:

		// Handlers
		std::unique_ptr<EnemyShipInputHandler> SIH;
		std::unique_ptr<EnemyShipMovementHandler> SMH;

		// Variables
		sf::Vector2f targetPosition;
		int groupID = -1;
	};
}

