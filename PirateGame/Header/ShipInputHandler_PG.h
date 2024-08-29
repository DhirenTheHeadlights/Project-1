#pragma once

/// This class contains the methods for handling the input
/// for the ship. This includes shooting, etc.

#include <SFML/Graphics.hpp>

#include "GlobalSoundManager_PG.h"
#include "GlobalWindController_PG.h"

#include "ShipCannonHandler_PG.h"
#include "ShipMovementHandler_PG.h"
#include "ShipSailHandler_PG.h"

namespace PirateGame {
	class ShipInputHandler {
	public:
		ShipInputHandler(JSONLoader* json, const sf::Sprite& sprite, GlobalSoundManager* GSM) : sprite(sprite), GSM(GSM), json(json) {}

		void update(const sf::Texture& cannonballTexture, GlobalIDManager* GIDM);
		virtual void handleCannonFire(const sf::Texture& cannonballTexture, GlobalIDManager* GIDM) = 0;
		virtual void handleCannonAim() = 0;
		virtual void handleAnchorDrop() = 0;
		virtual void handleSailChange() = 0;

		// Setters
		void setBaseSpeed(float baseSpeed) { this->baseSpeed = baseSpeed; }
		void setCannonHandler(ShipCannonHandler* SCH) { this->SCH = SCH; }
		void setMovementHandler(ShipMovementHandler* SMH) { this->SMH = SMH; }
		void setSailHandler(ShipSailHandler* SSH) { this->SSH = SSH; }
		void setInAudioRange(bool inAudioRange) { this->inAudioRange = inAudioRange; }
		void setCooldown(sf::Time cooldown) { this->cooldown = cooldown; }

		// Getters
		ShipCannonHandler* getCannonHandler() const { return SCH; }

	protected:
		GlobalSoundManager* GSM = nullptr;
		JSONLoader* json;

		const sf::Sprite& sprite;

		// Handlers
		ShipCannonHandler* SCH = nullptr;
		ShipMovementHandler* SMH = nullptr;
		ShipSailHandler* SSH = nullptr;

		// Ship properties
		float baseSpeed = 1.f;

		// Cooldown
		sf::Clock cannonCooldownClock;
		sf::Time cooldown = sf::seconds(0.5f);
		bool inAudioRange = false;
	};
}
