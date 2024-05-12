#include "ShipCannon_PG.h"

using namespace PirateGame;

void ShipCannon::fireCannon() {
    Cannonball* cannonball = new Cannonball(id);
    cannonball->getSprite().setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getMiscTextures().getTexture(MiscType::Cannonball));
    cannonball->getSprite().setScale(cannonballScale);
    cannonball->setSpeed(cannonballSpeed);

    // Considering a more accurate position calculation here based on cannon placement
    cannonball->setPos(cannonSprite.getPosition());
    cannonball->setVelocity(cannonball->getSpeed() * sf::Vector2f(1,1) /** cannonDirection*/);

    // Add the cannonball to the hashmap
    cannonballHashmap->addObject(cannonball);

    // Add the cannonball to the vector
    cannonballs.push_back(cannonball);
}

void ShipCannon::drawCannonballs() {
    for (auto& i : cannonballs) {
		GlobalValues::getInstance().getWindow()->draw(i->getSprite());
	}
}

void ShipCannon::updateCannonballs() {
    float elapsed = deltaTime.restart().asSeconds();
    for (auto it = cannonballs.begin(); it != cannonballs.end(); /* no increment here */) {
        // Update the cannonball in the hashmap
        cannonballHashmap->updateObjectPosition(*it);

        // Update the position and velocity (1% Decay) of the cannonball
        sf::Vector2f velocity = (*it)->getVelocity() * pow(0.97f, elapsed);
        (*it)->setVelocity(velocity);
        (*it)->setPos((*it)->getPos() + velocity * elapsed);

        // If more than 2 seconds have passed, delete the cannonball
        if ((*it)->getClock().getElapsedTime().asSeconds() > cannonballFlightTime || (*it)->getActive() == false) {
            // Remove the cannonball from the hashmap
            cannonballHashmap->removeObject(*it);

            // Delete the cannonball and erase it from the vector
            delete* it; // delete the object
            it = cannonballs.erase(it);
        }
        else {
            ++it;
        }
    }
}