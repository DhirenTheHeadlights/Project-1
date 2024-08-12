#pragma once

/// This class contains the methods for handling the input for enemy ships.

#include <SFML/Graphics.hpp>

#include "GlobalWindController_PG.h"

#include "ShipInputHandler_PG.h"

namespace PirateGame {
	class EnemyShipInputHandler : public ShipInputHandler {
	public:
		EnemyShipInputHandler(sf::Sprite& sprite, GlobalWindController* GWC, GlobalSoundManager* GSM) : ShipInputHandler(sprite, GSM), GWC(GWC) {};
		~EnemyShipInputHandler() {};

		void handleCannonFire(const sf::Texture& cannonBallTexture, GlobalIDManager* GIDM) override;
		void handleCannonAim() override;
		void handleAnchorDrop() override;
		void handleSailChange() override;

		// Setters
		void setTargetPos(sf::Vector2f targetPos) { 
			this->targetPos = targetPos;
			SCH->setTargetPos(targetPos);
		}
		void setFiringDistance(float firingDistance) { this->firingDistance = firingDistance; }
	private:
		GlobalWindController* GWC;

		sf::Vector2f targetPos;
		float firingDistance = 0.f;
	};
}
