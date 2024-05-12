#pragma oncwe
#include <SFML/Graphics.hpp>

#include "GlobalTextureHandler_PG.h"
#include "GlobalValues_PG.h"
#include "HashmapTemplate_PG.h"

#include "Cannonball_PG.h"


namespace PirateGame {

	class ShipCannon {

	public:
		ShipCannon(sf::Sprite& sprite, int id) : cannonSprite(sprite), id(id) {};
		~ShipCannon() {};

		void fireCannon();
		void drawCannonballs();
		void updateCannonballs();
	
	private:
		sf::Clock deltaTime;

		sf::Sprite& cannonSprite;
		std::vector<Cannonball*> cannonballs;
		sf::Vector2f cannonballScale = { 0.5f, 0.5f };

		float cannonballSpeed = 300;
		float cannonballFlightTime = 4.f;

		Hashmap<Cannonball>* cannonballHashmap = nullptr;

		int id;
	};
}
	