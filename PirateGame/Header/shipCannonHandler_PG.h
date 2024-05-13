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
		void setCooldown(float cooldown) { this->cooldown = sf::seconds(cooldown); };
		void setAimTowardsTarget(bool aimTowardsTarget) {
			for (auto& cannon : cannons) {
				cannon.setAimTowardsTarget(aimTowardsTarget);
			}
		};
		void setCannonballHashmap(Hashmap<Cannonball>* cannonballHashmap) {
			for (auto& cannon : cannons) {
				cannon.setCannonballHashmap(cannonballHashmap);
			}
		};
		void setInAudioRange(bool inAudioRange) { this->inAudioRange = inAudioRange; };

		// Getters
		bool getAimTowardsMouse() const { return aimTowardsMouse; };
		FiringSide getFiringSide() const { return side; };
		float getFiringDirectionAngle() const {
			// Average the firing angles of all the cannons on the firing side
			float firingAngle = 0.f;
			for (auto& cannon : cannons) {
				if (cannon.getFiringSide() == side) {
					firingAngle += cannon.getFiringDirectionAngle();
				}
			}
			return firingAngle / cannons.size() / 2.f; // Divide by 2 for only one side
		}
		float getMaxFiringAngle() const { return maxFiringAngle; };
	private:
		sf::Clock cannonCooldownClock;
		sf::Time cooldown = sf::seconds(0.5f);

		sf::Vector2f cannonballDirection;
		std::vector<ShipCannon> cannons;
		
		const float maxFiringAngle = 45.f;

		bool inAudioRange = false;
		bool aimTowardsMouse = false;

		sf::Sprite& shipSprite;
		FiringSide side = FiringSide::Port;
	};
};