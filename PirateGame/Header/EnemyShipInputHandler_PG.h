#pragma once

/// This class contains the methods for handling the input for enemy ships.

#include <SFML/Graphics.hpp>

#include "WindHandler_PG.h"

#include "ShipInputHandler_PG.h"

namespace PirateGame {
	class EnemyShipInputHandler : public ShipInputHandler {
	public:
		EnemyShipInputHandler(JSONLoader* json, sf::Sprite& sprite, GlobalSoundManager* GSM) : ShipInputHandler(json, sprite, GSM) {};
		~EnemyShipInputHandler() {};

		void handleCannonFire(const sf::Texture& cannonBallTexture, GlobalIDManager* GIDM) override;
		void handleCannonAim() override;
		void handleAnchorDrop() override;
		void handleSailChange(const sf::Vector2f& windDirection) override;

		// Setters
		void setTargetPos(sf::Vector2f targetPos) { 
			this->targetPos = targetPos;
			SCH->setTargetPos(targetPos);
		}
		void setFiringDistance(float firingDistance) { this->firingDistance = firingDistance; }
	private:


		sf::Vector2f targetPos;
		float firingDistance = 0.f;
	};
}
