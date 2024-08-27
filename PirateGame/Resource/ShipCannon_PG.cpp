#include "ShipCannon_PG.h"

using namespace PirateGame;

void ShipCannon::fireCannon(FiringSide FS, const sf::Sprite& shipSprite, const sf::Texture& cannonballTexture, GlobalIDManager* GIDM) {
    if (FS != side) return;

    Cannonball* cannonball = new Cannonball(GIDM, id);
    cannonball->getSprite().setTexture(cannonballTexture);
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

void ShipCannon::drawCannonNBalls(sf::RenderWindow* window) {
    for (auto& i : cannonballs) {
		window->draw(i->getSprite());
	}
    window->draw(cannonSprite);
}

sf::Vector2f ShipCannon::calculatePerpendicularDirection(float rotation) const {
    float rotationInRadians = vm::degreesToRadians(rotation);

    // Calculate the direction of the cannon and normalize it
    sf::Vector2f cannonDirection = vm::normalize(vm::angleRadiansToVector(rotationInRadians));

    cannonDirection *= (side == FiringSide::Starboard) ? -1.f : 1.f;

    return cannonDirection;
}

sf::Vector2f ShipCannon::calculateDirectionToTarget(const sf::Sprite& shipSprite, sf::Vector2f targetPos) {
    sf::Vector2f shipPos = shipSprite.getPosition();

    // Calculate vector from ship to target
    sf::Vector2f directionToTarget = targetPos - shipPos;
    directionToTarget *= (side == FiringSide::Starboard) ? -1.f : 1.f;

    // Calculate angle from ship to target in degrees
    float angleToMouse = vm::vectorToAngleDegrees(directionToTarget);

    // Adjust angle based on ship orientation and capped angle
    float shipRotation = (shipSprite.getRotation() - 180);
    float angleDifference = vm::normalizeAngle((angleToMouse - shipRotation), -180.f, 180.f);

    // Cap the angleDifference within the maxAngle
    angleDifference = std::max(std::min(angleDifference, maxFiringAngle), -maxFiringAngle);

    // Calculate final direction vector based on capped angle
    float cappedAngleRadians = vm::degreesToRadians(shipRotation + angleDifference);
    sf::Vector2f cappedDirection = vm::angleRadiansToVector(cappedAngleRadians);

    // Normalize the capped direction vector
    cappedDirection = vm::normalize(cappedDirection);

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

void ShipCannon::updateCannonRotation(const sf::Sprite& shipSprite, FiringSide FS, sf::RenderWindow* window) {
    if (FS != side) {
        state = FiringState::Untargeted;
    }

    // Calculate the cannon's position based on the ship's rotation
    float rotation = shipSprite.getRotation();
    sf::Transform rotationTransform;
    rotationTransform.rotate(rotation, shipSprite.getPosition());

    sf::Vector2f rotationPoint(shipSprite.getPosition() + offset);
    sf::Vector2f cannonPosition = rotationTransform.transformPoint(rotationPoint);
    cannonSprite.setPosition(cannonPosition);

    switch (state) {
        case FiringState::TowardsTarget: {
            sf::Vector2f targetDirection = calculateDirectionToTarget(shipSprite, targetPos);
            float targetRotation = vm::vectorToAngleDegrees(targetDirection);
            rotateTowards(targetRotation, rotationSpeed);

            resetRotationClock.restart();
            break;
        }
        case FiringState::TowardsMouse: {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
            sf::Vector2f worldMousePos = window->mapPixelToCoords(mousePosition);
            sf::Vector2f targetDirection = calculateDirectionToTarget(shipSprite, worldMousePos);
            float targetRotation = vm::vectorToAngleDegrees(targetDirection);

            rotateTowards(targetRotation, rotationSpeed);

            resetRotationClock.restart();
            break;
        }
        case FiringState::Untargeted: {
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
}

void ShipCannon::updateCannonballs(sf::Time elapsed) {
    for (auto it = cannonballs.begin(); it != cannonballs.end(); /* no increment here */) {
        // Update the cannonball in the hashmap
        cannonballHashmap->updateObjectPosition(*it);

        // Update the position and velocity (1% Decay) of the cannonball
        sf::Vector2f velocity = (*it)->getVelocity() * pow(0.97f, elapsed.asSeconds());
        (*it)->setVelocity(velocity);
        (*it)->getSprite().setPosition((*it)->getSprite().getPosition() + velocity * elapsed.asSeconds());

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

void ShipCannon::updateCannon(const sf::Sprite& shipSprite, FiringSide FS, sf::RenderWindow* window, sf::Time elapsed) {
    updateCannonRotation(shipSprite, FS, window);
    updateCannonballs(elapsed);
}