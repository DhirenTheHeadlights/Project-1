#pragma	once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

#include "GlobalValues_PG.h"
#include "GlobalSoundManager_PG.h"
#include "GlobalTextureHandler_PG.h"
#include "HashmapTemplate_PG.h"

#include "ShipCannon_PG.h"
#include "Cannonball_PG.h"

namespace PirateGame {
	class ShipCannonHandler {
	public:
		ShipCannonHandler(sf::Sprite& shipSprite) : shipSprite(shipSprite) {};
		~ShipCannonHandler() {};

		void initializeCannons(ShipClass type, int numCannons, int ID, sf::Vector2f scale);

		void shootCannonballs(sf::Vector2f targetPos);
		void updateCannons();
		void drawCannons();

		// Setters
		void setFiringSide(FiringSide side) { this->side = side; };
		void setCooldown(float cooldown) { this->cooldown = cooldown; };
		void setFreeAim(bool aimTowardsMouse) { this->aimTowardsMouse = aimTowardsMouse; };
		void setCannonballHashmap(Hashmap<Cannonball>* cannonballHashmap) {
			for (auto& cannon : cannons) {
				cannon.setCannonballHashmap(cannonballHashmap);
			}
		};
		void setInAudioRange(bool inAudioRange) { this->inAudioRange = inAudioRange; };

		// Getters
		bool getAimTowardsMouse() const { return aimTowardsMouse; };
		FiringSide getFiringSide() const { return side; };
		float getFiringDirectionAngle() const { return firingDirectionAngle; };
		float getMaxFiringAngle() const { return maxFiringAngle; };
	private:
		sf::Clock cannonCooldownClock;

		sf::Vector2f cannonballDirection;
		std::vector<ShipCannon> cannons;
		
		bool aimTowardsMouse = false;
		const float maxFiringAngle = 45.f;
		float cooldown = 0.1f;
		float firingDirectionAngle = 0;
		bool inAudioRange = false;

		sf::Sprite& shipSprite;
		FiringSide side = FiringSide::Port;

		sf::Vector2f cannonDirection(sf::Vector2f targetPos);
	};
};