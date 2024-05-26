#include "ShipCannon_PG.h"

using namespace PirateGame;

void ShipCannon::fireCannon(FiringSide FS, sf::Sprite& shipSprite) {
    if (FS != side) return;

    Cannonball* cannonball = new Cannonball(id);
    cannonball->getSprite().setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getMiscTextures().getTexture(MiscType::Cannonball));
    cannonball->getSprite().setScale(cannonballScale);
    cannonball->setSpeed(cannonballSpeed);

    // Set the cannonball to come out of the cannon's muzzle
    sf::Vector2f muzzleOffset;
    sf::Vector2f currentFireDirection = vm::angleDegreesToVector(cannonSprite.getRotation());
    muzzleOffset.x = cannonSprite.getGlobalBounds().width * approxCannonOffsetToEdgeRatio * currentFireDirection.x;
    muzzleOffset.y = 0.f * currentFireDirection.y;  // This will always be zero since second component is zero

    // Adjust the muzzleOffset to also consider the size of the cannonball sprite
    muzzleOffset -= (sf::Vector2f(0.f, cannonball->getSprite().getGlobalBounds().height / 2.f * (FS == FiringSide::Starboard ? -1.f : 1.f)));
    
    cannonball->getSprite().setPosition(cannonSprite.getPosition() + muzzleOffset);
    cannonball->setVelocity(currentFireDirection * cannonballSpeed);

    // Add the cannonball to the management systems
    cannonballHashmap->addObject(cannonball);
    cannonballs.push_back(cannonball);
}

void ShipCannon::drawCannonNBalls() {
    sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
    for (auto& i : cannonballs) {
		window->draw(i->getSprite());
	}
    window->draw(cannonSprite);
}

sf::Vector2f ShipCannon::calculatePerpendicularDirection(float rotation) const {
    float rotationInRadians = (rotation - 180.f) * pi / 180.f;

    // Calculate the direction of the cannon and normalize it
    sf::Vector2f cannonDirection(cos(rotationInRadians), sin(rotationInRadians));
    float magnitude = sqrt(cannonDirection.x * cannonDirection.x + cannonDirection.y * cannonDirection.y);
    cannonDirection /= magnitude;

    cannonDirection *= (side == FiringSide::Starboard) ? -1.f : 1.f;

    return cannonDirection;
}

sf::Vector2f ShipCannon::calculateDirectionToTarget(sf::Sprite& shipSprite, sf::Vector2f targetPos) {
    const float pi = 3.14159265f;
    sf::Vector2f shipPos = shipSprite.getPosition();

    // Calculate vector from ship to target
    sf::Vector2f directionToTarget = targetPos - shipPos;
    directionToTarget *= (side == FiringSide::Starboard) ? -1.f : 1.f;

    // Calculate angle from ship to target in degrees
    float angleToMouse = atan2(directionToTarget.y, directionToTarget.x) * 180 / pi;

    // Adjust angle based on ship orientation and capped angle
    float shipRotation = (shipSprite.getRotation() - 180);
    float angleDifference = vm::normalizeAngle((angleToMouse - shipRotation), -180.f, 180.f);

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

// Rotate the cannon towards a target angle smoothly
void ShipCannon::rotateTowards(float targetAngle, float step) {
    float currentAngle = cannonSprite.getRotation();
    float angleDifference = vm::normalizeAngle((targetAngle - currentAngle), -180.f, 180.f);

    if (std::fabs(angleDifference) < step) {
        cannonSprite.setRotation(targetAngle);
    }
    else {
        cannonSprite.rotate(step * (angleDifference > 0 ? 1 : -1));
    }
}

void ShipCannon::updateCannonRotation(sf::Sprite& shipSprite, FiringSide FS) {
    // Calculate the cannon's position based on the ship's rotation
    float rotation = shipSprite.getRotation();
    sf::Transform rotationTransform;
    rotationTransform.rotate(rotation, shipSprite.getPosition());

    sf::Vector2f rotationPoint(shipSprite.getPosition() + offset);
    sf::Vector2f cannonPosition = rotationTransform.transformPoint(rotationPoint);
    cannonSprite.setPosition(cannonPosition);

    switch (state) {
        case FiringState::TowardsTarget:
            if (FS == side) {
				sf::Vector2f targetDirection = calculateDirectionToTarget(shipSprite, targetPos);
				float targetRotation = atan2(targetDirection.y, targetDirection.x) * 180 / pi;
				rotateTowards(targetRotation, rotationSpeed);
			}
			break;
        case FiringState::TowardsMouse:
            if (FS == side) {
		        sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
		        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
		        sf::Vector2f worldMousePos = window->mapPixelToCoords(mousePosition);
		        sf::Vector2f targetDirection = calculateDirectionToTarget(shipSprite, worldMousePos);
		        float targetRotation = atan2(targetDirection.y, targetDirection.x) * 180 / pi;

		        rotateTowards(targetRotation, rotationSpeed);

		        resetRotationClock.restart();
		    }
            break;
        case FiringState::Untargeted:
            float targetRotation = rotation + defaultRotation;
			// If firing state is untargeted, slowly rotate towards the default rotation
            if (resetRotationClock.getElapsedTime() < resetRotationTime) {
                rotateTowards(rotation + defaultRotation, rotationSpeed);
            }
            // Now, rotate towards the default rotation immediately so that the cannon isnt affected by the ship's rotation
			else cannonSprite.setRotation(targetRotation);
            break;
    }
}

void ShipCannon::updateCannonballs(float elapsed) {
    for (auto it = cannonballs.begin(); it != cannonballs.end(); /* no increment here */) {
        // Update the cannonball in the hashmap
        cannonballHashmap->updateObjectPosition(*it);

        // Update the position and velocity (1% Decay) of the cannonball
        sf::Vector2f velocity = (*it)->getVelocity() * pow(0.97f, elapsed);
        (*it)->setVelocity(velocity);
        (*it)->getSprite().setPosition((*it)->getSprite().getPosition() + velocity * elapsed);

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

void ShipCannon::updateCannon(sf::Sprite& shipSprite, FiringSide FS) {
    updateCannonRotation(shipSprite, FS);

    float elapsed = deltaTime.restart().asSeconds();
    updateCannonballs(elapsed);
}