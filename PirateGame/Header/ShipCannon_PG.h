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
		ShipCannon(ShipClass type, int id, FiringSide FS, sf::Vector2f scale) : id(id) {
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

		void fireCannon(sf::Vector2f cannonDirection);
		void drawCannonballs();
		void updateCannon(sf::Sprite& shipSprite);
		void drawCannon() const { GlobalValues::getInstance().getWindow()->draw(cannonSprite); }

		// Setters
		void setCannonballHashmap(Hashmap<Cannonball>* cannonballHashmap) { this->cannonballHashmap = cannonballHashmap; }
		void setOffset(sf::Vector2f offset) { this->offset = offset; }

		// Getters
		sf::Sprite& getSprite() { return cannonSprite; }
		sf::Vector2f getOffset() const { return offset; }
	private:
		sf::Clock deltaTime;

		sf::Sprite cannonSprite;
		std::vector<Cannonball*> cannonballs;
		sf::Vector2f cannonballScale = { 0.5f, 0.5f };
		sf::Vector2f offset = { 0.f, 0.f };

		float cannonballSpeed = 300;
		float cannonballFlightTime = 4.f;
		float defaultRotation = 0.f;

		Hashmap<Cannonball>* cannonballHashmap = nullptr;

		int id;
	};
}
	