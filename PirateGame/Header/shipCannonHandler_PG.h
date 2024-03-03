#pragma	once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

#include "GlobalValues_PG.h"
#include "Cannonball_PG.h"
#include "SoundManager_PG.h"
#include "Textures_PG.h"

namespace PirateGame {

	enum class FiringSide { Port, Starboard };

	class ShipCannonHandler {
	public:
		ShipCannonHandler(sf::Sprite& shipSprite) : shipSprite(shipSprite), soundManager(soundFile) {};
		~ShipCannonHandler() {};

		void shootCannonballs(int numCannons);
		void updateCannonballs();
		void drawCannonballs();

		// Setters
		void setFiringSide(FiringSide side) { this->side = side; };
		void setCooldown(float cooldown) { this->cooldown = cooldown; };
		void setAimTowardsMouse(bool aimTowardsMouse) { this->aimTowardsMouse = aimTowardsMouse; };

		// Getters
		bool getAimTowardsMouse() const { return aimTowardsMouse; };
		FiringSide getFiringSide() const { return side; };
	private:
		std::vector<Cannonball*> cannonballs{};
		sf::Clock cannonCooldownClock;
		sf::Clock deltaTime;

		sf::Vector2f cannonballDirection;
		sf::Vector2f cannonballScale = { 0.5f, 0.5f };
		
		bool aimTowardsMouse = false;
		float cooldown = 0.1f;
		float cannonballSpeed = 1000;

		sf::Sprite& shipSprite;
		FiringSide side = FiringSide::Port;

		std::string soundFile = "Sounds/cannonshot.wav";
		SoundManager soundManager;

		Textures textures;

		sf::Vector2f cannonDirection();
	};
};