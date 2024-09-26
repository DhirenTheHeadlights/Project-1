#include "EnemyShipMovementHandler_PG.h"


using namespace PirateGame;

void EnemyShipMovementHandler::update(const sf::Vector2f& sailDirection, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed) {
    astar.update(sprite.getPosition());
    //getAStar().drawDebug(Globals::window);

	move(sailDirection, dt, windDirection, windSpeed);
}

void EnemyShipMovementHandler::setSpriteRotation() {
    if (getStopShipRotationFlag()) return;

    //Globals::window->draw(vm::createVectorLine(getSprite().getPosition(), vm::normalize(destination - getSprite().getPosition()) * 200.f, sf::Color::Green));
    //Globals::window->draw(vm::createVectorLine(getSprite().getPosition(), vm::normalize(direction) * 200.f, sf::Color::Red));

    // Check if the ship is active towards the target
    if (activeTowardsTarget) {
        sf::Vector2f directionToTarget = targetPos - sprite.getPosition();
        float distance = vm::magnitude(directionToTarget);
        if (distance < broadsideDistance) { // Go perpendicular
            direction = sf::Vector2f(directionToTarget.y, -directionToTarget.x);
        }
    }
    else {
        // Get the next point from the A* algorithm
        direction = astar.getNextPoint(sprite.getPosition()) - sprite.getPosition();
    }

    setTurningMultiplier(activeTowardsTarget ? 1.5f : 1.f);

    rotateTowards(vm::normalizeAngle(vm::vectorToAngleDegrees(direction) + 90.f)); // Add 90 degrees to align with SFML's rotation
}
