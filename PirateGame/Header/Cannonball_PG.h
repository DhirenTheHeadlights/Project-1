#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalIDManager_PG.h"

namespace PirateGame {
	struct Cannonball {
		Cannonball(GlobalIDManager* GIDM, ID* shipID, sf::Texture& texture, sf::Vector2f position, sf::Vector2f normalizedVelocity, float speed) :
			shipID(shipID), id(GIDM->generateID().get()), sprite(texture), velocity(normalizedVelocity), speed(speed) {
			sprite.setPosition(position);
		}

		bool operator==(const Cannonball& other) const {
			return this->id == other.id;
		}

		sf::Clock clock;

		sf::Vector2f velocity;
		sf::Sprite sprite;

		float speed = 0;

		ID* shipID;
		ID* id;

		bool isActive = true;
	};
};
