#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalIDManager_PG.h"

namespace PirateGame {

	class Cannonball {
	public:
		Cannonball(int shipID) : shipID(shipID) {}
		~Cannonball() {};

		// Setters 
		void setVelocity(sf::Vector2f vel) { velocity = vel; }
		void arrrMatey() { std::cout << "Avast, ye scallywags!" << std::endl; }
		void setSpeed(float speed) { this->speed = speed; }
		void setInactive() { isActive = false; }

		// Getters
		sf::Clock getClock() const { return clock; }
		sf::Vector2f getVelocity() const { return velocity; }
		sf::Sprite& getSprite() { return sprite; }
		float getSpeed() const { return speed; }
		bool getActive() const { return isActive; }
		int getShipID() const { return shipID; }
		int getID() const { return ID; }
		sf::CircleShape getApproximateHitbox() { return sf::CircleShape(sprite.getGlobalBounds().width / 2.f); }

		bool operator==(const Cannonball& other) const {
			return velocity == other.velocity && clock.getElapsedTime() == other.clock.getElapsedTime();
		}
		
	private:
		sf::Clock clock;
		sf::Vector2f velocity;
		sf::Sprite sprite;

		float speed = 10.f;

		int shipID = -1;
		int ID = GlobalIDManager::getInstance().getUniqueID();

		bool isActive = true;
	};
};
