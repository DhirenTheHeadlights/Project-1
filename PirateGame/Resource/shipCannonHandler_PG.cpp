#include "shipCannonHandler_PG.h"

using namespace PirateGame;

void ShipCannonHandler::shootCannonballs(int numCannons) {
	// If the ship is not ready to fire, return
	if (cannonCooldownClock.getElapsedTime().asSeconds() < cooldown) return;

	// Get the rotation of the ship and convert to radians
	float rotationInRadians = (shipSprite.getRotation() - 180) * 3.14159265f / 180.f;

	// Calculate the direction of the cannon
	sf::Vector2f cannonDirection(cos(rotationInRadians), sin(rotationInRadians));
	cannonDirection = cannonDirection / static_cast<float>((sqrt(pow(cannonDirection.x, 2) + pow(cannonDirection.y, 2))));

	switch (side) {
		case FiringSide::Right:
			cannonDirection = sf::Vector2f(-cannonDirection.x, -cannonDirection.y);
			break;
		case FiringSide::Left:
			break;
	}

	// For each cannon, create a cannonball and set its position and velocity, add to vector
	for (int i = 0; i < numCannons; i++) {
		Cannonball* cannonball = new Cannonball;
		cannonball->setPos(shipSprite.getPosition());
		cannonball->setVelocity(cannonball->getSpeed() * cannonDirection);
		cannonball->getSprite().setScale(cannonballScale);
		cannonballs.push_back(cannonball);
	}

	// Reset the cooldown clock
	cannonCooldownClock.restart();
}


void ShipCannonHandler::updateCannonballs() {
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

	for (auto it = cannonballs.begin(); it != cannonballs.end(); /* no increment here */) {
		// Update the position and velocity (1% Decay) of the cannonball
		(*it)->setVelocity((*it)->getVelocity() * 0.99f);
		(*it)->setPos((*it)->getPos() + (*it)->getVelocity());
		window->draw((*it)->getSprite());

		// If more than 2 seconds have passed, delete the cannonball
		if ((*it)->getClock().getElapsedTime().asSeconds() > 2) {
			delete* it; // delete the object
			it = cannonballs.erase(it);
		}
		else {
			++it;
		}
	}
}