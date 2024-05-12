#pragma	once
#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "Cannonball_PG.h"


namespace PirateGame {
	class Cannon {
	public:
		Cannon(sf::Sprite& sprite) : cannonSprite(sprite)  {};
		~Cannon() {};
	private:
		sf::Sprite& cannonSprite;

	};

}
