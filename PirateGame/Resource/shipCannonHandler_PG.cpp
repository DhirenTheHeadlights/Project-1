#include "shipCannonHandler_PG.h"

using namespace PirateGame;

void ShipCannonHandler::shootCannonballs(int numCannons, sf::Vector2f targetPos) {
    if (cannonCooldownClock.getElapsedTime().asSeconds() < cooldown) return;

    // Grab the cannon direction
    sf::Vector2f cannonDirection = this->cannonDirection(targetPos);

    // Fire the cannonballs with the adjusted direction
    for (int i = 0; i < numCannons; i++) {
        Cannonball* cannonball = new Cannonball;
        cannonball->getSprite().setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getCannonballTexture());
        cannonball->getSprite().setScale(cannonballScale);
        cannonball->setSpeed(cannonballSpeed);

        // Considering a more accurate position calculation here based on cannon placement
        float padding = 10.f;
        cannonball->setPos(shipSprite.getPosition() + sf::Vector2f(static_cast<float>(i * padding), static_cast<float>(i * padding)));
        cannonball->setVelocity(cannonball->getSpeed() * cannonDirection);

        // Add the cannonball to the hashmap
        //GlobalHashmapHandler::getInstance().getCannonballHashmap()->addObject(cannonball);

        // Add the cannonball to the vector
        cannonballs.push_back(cannonball);
    }

    // Play sound once per volley if numCannons > 0
    if (numCannons > 0) GlobalSoundManager::getInstance().playSound(SoundId::CannonShot);

    // Reset the cooldown clock
    cannonCooldownClock.restart();
}

sf::Vector2f ShipCannonHandler::cannonDirection(sf::Vector2f targetPos) {
    const float pi = 3.14159265f;
    if (!aimTowardsMouse) {
        float rotationInRadians = (shipSprite.getRotation() - 180.f) * pi / 180.f;

        // Calculate the direction of the cannon and normalize it
        sf::Vector2f cannonDirection(cos(rotationInRadians), sin(rotationInRadians));
        float magnitude = sqrt(cannonDirection.x * cannonDirection.x + cannonDirection.y * cannonDirection.y);
        cannonDirection /= magnitude;

        cannonDirection *= (side == FiringSide::Starboard) ? -1.f : 1.f;

        return cannonDirection;
    }
    else {
        sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
        sf::Vector2f shipPos = shipSprite.getPosition();

        // Calculate vector from ship to target
        sf::Vector2f directionToTarget = targetPos - shipPos;
        directionToTarget *= (side == FiringSide::Starboard) ? -1.f : 1.f;

        // Calculate angle from ship to target in degrees
        float angleToMouse = atan2(directionToTarget.y, directionToTarget.x) * 180 / pi;

        // Adjust angle based on ship orientation and capped angle
        float shipRotation = (shipSprite.getRotation() - 180);
        float angleDifference = angleToMouse - shipRotation;

        // Ensure angleDifference is within -180 to 180 degrees for correct comparison
        if (angleDifference > 180) angleDifference -= 360;
        else if (angleDifference < -180) angleDifference += 360;

        firingDirectionAngle = angleDifference; // This value is grabbed by the enemy ship input handler
 
        // Cap the angleDifference within the maxAngle
        angleDifference = std::max(std::min(angleDifference, maxFiringAngle), -maxFiringAngle);

        // Calculate final direction vector based on capped angle
        float cappedAngleRadians = (shipRotation + angleDifference) * pi / 180;
        sf::Vector2f cappedDirection(cos(cappedAngleRadians), sin(cappedAngleRadians));

        // Normalize the capped direction vector
        float magnitude = sqrt(cappedDirection.x * cappedDirection.x + cappedDirection.y * cappedDirection.y);
        cappedDirection /= magnitude;

        // Invert the direction if firing from the starboard side
        cappedDirection *= (side == FiringSide::Starboard) ? -1.f : 1.f;

        return cappedDirection;
    }
}

void ShipCannonHandler::updateCannonballs() {
    float elapsed = deltaTime.restart().asSeconds();
	for (auto it = cannonballs.begin(); it != cannonballs.end(); /* no increment here */) {
        // Update the cannonball in the hashmap
        //GlobalHashmapHandler::getInstance().getCannonballHashmap()->updateObjectPosition(*it);

		// Update the position and velocity (1% Decay) of the cannonball
		sf::Vector2f velocity = (*it)->getVelocity() * pow(0.97f, elapsed);
		(*it)->setVelocity(velocity);
		(*it)->setPos((*it)->getPos() + velocity * elapsed);

		// If more than 2 seconds have passed, delete the cannonball
		if ((*it)->getClock().getElapsedTime().asSeconds() > 2 || (*it)->getActive() == false) {
            // Remove the cannonball from the hashmap
            //GlobalHashmapHandler::getInstance().getCannonballHashmap()->removeObject(*it);

            // Delete the cannonball and erase it from the vector
			delete* it; // delete the object
			it = cannonballs.erase(it);
		}
		else {
			++it;
		}
	}
}

void ShipCannonHandler::drawCannonballs() {
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
    for (auto it = cannonballs.begin(); it != cannonballs.end(); ++it) {
		window->draw((*it)->getSprite());
	}
}