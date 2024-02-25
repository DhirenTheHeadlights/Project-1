#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalValues_PG.h"
#include "Textures_PG.h"
#include "ShipMovementHandler_PG.h"


namespace PirateGame {

	class Cannonball {
	public:
		Cannonball();
		~Cannonball() {};

		void setPos(sf::Vector2f pos) { sprite.setPosition(pos); }
		void setVelocity(sf::Vector2f vel) { velocity = vel; }
		void arrrMatey() { std::cout << "Avast, ye scallywags!" << std::endl; }

		sf::Clock getClock() { return clock; }
		sf::Vector2f getPos() { return sprite.getPosition(); }
		sf::Vector2f getVelocity() { return velocity; }
		sf::Sprite& getSprite() { return sprite; }
		float getSpeed() { return speed; }

		bool operator==(const Cannonball& other) const {
			return velocity == other.velocity && clock.getElapsedTime() == other.clock.getElapsedTime();
		}
		
	private:
		float speed = 10.f;
		sf::Clock clock;
		sf::Vector2f velocity;
		sf::Sprite sprite;
		Textures textures;
		sf::Texture texture;
	};
};
