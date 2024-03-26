#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalValues_PG.h"
#include "TextureHandler_PG.h"
#include "ShipMovementHandler_PG.h"

namespace PirateGame {

	class Cannonball {
	public:
		Cannonball(int ID) : ID(ID) {};
		~Cannonball() {};

		// Setters
		void setPos(sf::Vector2f pos) { sprite.setPosition(pos); }
		void setVelocity(sf::Vector2f vel) { velocity = vel; }
		void arrrMatey() { std::cout << "Avast, ye scallywags!" << std::endl; }
		void setSpeed(float speed) { this->speed = speed; }
		void setInactive() { isActive = false; }

		// Getters
		sf::Clock getClock() const { return clock; }
		sf::Vector2f getPos() const { return sprite.getPosition(); }
		sf::Vector2f getVelocity() const { return velocity; }
		sf::Sprite& getSprite() { return sprite; }
		float getSpeed() const { return speed; }
		bool getActive() const { return isActive; }
		int getID() const { return ID; }

		bool operator==(const Cannonball& other) const {
			return velocity == other.velocity && clock.getElapsedTime() == other.clock.getElapsedTime();
		}
		
	private:
		sf::Clock clock;
		sf::Vector2f velocity;
		sf::Sprite sprite;

		float speed = 10.f;

		int ID = -1;

		bool isActive = true;
	};
};
