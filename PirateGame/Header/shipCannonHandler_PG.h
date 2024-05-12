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

namespace PirateGame {

	enum class FiringSide { Port, Starboard };

	class ShipCannonHandler {
	public:
		ShipCannonHandler(sf::Sprite& shipSprite) : shipSprite(shipSprite) {};
		~ShipCannonHandler() {};

		void shootCannonballs(int numCannons, sf::Vector2f targetPos);
		void updateCannonballs();
		void drawCannonballs();

		// Setters
		void setFiringSide(FiringSide side) { this->side = side; };
		void setCooldown(float cooldown) { this->cooldown = cooldown; };
		void setFreeAim(bool aimTowardsMouse) { this->aimTowardsMouse = aimTowardsMouse; };
		void setCannonballHashmap(Hashmap<Cannonball>* cannonballHashmap) { this->cannonballHashmap = cannonballHashmap; };
		void setID(int ID) { this->ID = ID; };
		void setInAudioRange(bool inAudioRange) { this->inAudioRange = inAudioRange; };

		// Getters
		bool getAimTowardsMouse() const { return aimTowardsMouse; };
		FiringSide getFiringSide() const { return side; };
		float getFiringDirectionAngle() const { return firingDirectionAngle; };
		float getMaxFiringAngle() const { return maxFiringAngle; };
	private:
		std::vector<Cannonball*> cannonballs{};
		sf::Clock cannonCooldownClock;
		sf::Clock deltaTime;

		sf::Vector2f cannonballDirection;
		sf::Vector2f cannonballScale = { 0.5f, 0.5f };
		
		bool aimTowardsMouse = false;
		const float maxFiringAngle = 45.f;
		float cooldown = 0.1f;
		float cannonballSpeed = 300;
		float cannonballFlightTime = 4.f;
		float firingDirectionAngle = 0;
		bool inAudioRange = false;

		sf::Sprite& shipSprite;
		FiringSide side = FiringSide::Port;

		sf::Vector2f cannonDirection(sf::Vector2f targetPos);

		int ID = -1;

		Hashmap<Cannonball>* cannonballHashmap = nullptr;
	};
};