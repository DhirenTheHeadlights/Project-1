#pragma	once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

#include "GlobalValues_PG.h"
#include "Cannonball_PG.h"

namespace PirateGame {

	enum class FiringSide { Left, Right };

	class ShipCannonHandler {
	public:
		ShipCannonHandler(sf::Sprite& shipSprite) : shipSprite(shipSprite) {};
		~ShipCannonHandler() {};

		void shootCannonballs(int numCannons);
		void updateCannonballs();

		void setFiringSide(FiringSide side) { this->side = side; };
		void setCooldown(float cooldown) { this->cooldown = cooldown; };
	private:
		std::vector<Cannonball*> cannonballs{};

		sf::Vector2f cannonballDirection;
		sf::Vector2f cannonballScale = { 0.5f, 0.5f };

		sf::Sprite& shipSprite;
		sf::Clock cannonCooldownClock;

		FiringSide side = FiringSide::Left;

		float cooldown = 0.1f;
	};
};