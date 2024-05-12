#pragma once
#include <SFML/Graphics.hpp>

#include "GlobalTextureHandler_PG.h"
#include "GlobalValues_PG.h"
#include "HashmapTemplate_PG.h"

#include "Cannonball_PG.h"
#include "ShipEnums_PG.h"


namespace PirateGame {
	class ShipCannon {
	public:
		ShipCannon(ShipClass type, int id, FiringSide FS, sf::Vector2f scale) : id(id), side(FS) {
			cannonSprite.setTexture(GlobalTextureHandler::getInstance().getShipTextures().getCannonTextureManager().getTexture(type));
			cannonSprite.setScale(scale);

			if (FS == FiringSide::Port) {
				defaultRotation = 180.f;
				// The origin is not perfectly in the center of the cannon
				// so we need to adjust it slightly (the cannon sprite has
				// an even number of pixels in width and height)
				cannonSprite.setOrigin(7.f, 7.f); 
			}
			else {
				defaultRotation = 0.f;
				cannonSprite.setOrigin(5.f, 5.f);
			}
		};
		~ShipCannon() {};

		void fireCannon(FiringSide FS, sf::Sprite& shipSprite, sf::Vector2f targetPos);
		void drawCannonNBalls();
		void updateCannon(sf::Sprite& shipSprite, FiringSide FS);

		// Setters
		void setCannonballHashmap(Hashmap<Cannonball>* cannonballHashmap) { this->cannonballHashmap = cannonballHashmap; }
		void setOffset(sf::Vector2f offset) { this->offset = offset; }
		void setAimTowardsMouse(bool aimTowardsMouse) { this->aimTowardsMouse = aimTowardsMouse; }
	
		// Getters
		sf::Sprite& getSprite() { return cannonSprite; }
		sf::Vector2f getOffset() const { return offset; }
		float getFiringDirectionAngle() const { return firingDirectionAngle; }
		FiringSide getFiringSide() const { return side; }
	private:
		// Helpers
		sf::Vector2f calculateDirectionToTarget(sf::Sprite& shipSprite, sf::Vector2f targetPos);
		float normalizeAngle(float angle);
		void rotateTowards(float angle, float step);
		void updateCannonRotation(sf::Sprite& shipSprite, FiringSide FS);
		void updateCannonballs(float elapsed);

		sf::Clock resetRotationClock;
		sf::Time resetRotationTime = sf::seconds(2.f);
		sf::Clock deltaTime;

		sf::Sprite cannonSprite;
		std::vector<Cannonball*> cannonballs;
		sf::Vector2f cannonballScale = { 0.4f, 0.4f };
		sf::Vector2f offset = { 0.f, 0.f };

		const float approxCannonOffsetToEdgeRatio = 0.85f;
		const float pi = 3.14159265f;
		float cannonballSpeed = 300;
		float cannonballFlightTime = 4.f;
		float defaultRotation = 0.f;
		float maxFiringAngle = 45.f;
		float firingDirectionAngle = 0.f;
		float aimRotation = 0.f;
		float rotationSpeed = 0.5f;
		float minDifferenceBetweenTargetAndCannon = 0.01f;

		bool aimTowardsMouse = false;

		FiringSide side;

		Hashmap<Cannonball>* cannonballHashmap = nullptr;

		int id;
	};
}
	