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

		void shootCannonballs(int numCannons, FiringSide side);
		void updateCannonballs();
	private:
		std::vector<Cannonball> cannonballs{};
		sf::Vector2f cannonballDirection;
		sf::Sprite& shipSprite;
		FiringSide side;

	};
};