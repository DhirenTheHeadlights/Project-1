#include "EnemyShipInputHandler_PG.h"

using namespace PirateGame;

void EnemyShipInputHandler::handleCannonFire() {
	// Only fire if the player is within firingRange
	float distance = sqrt(pow(playerPos.x - sprite.getPosition().x, 2) + pow(playerPos.y - sprite.getPosition().y, 2));
	if (distance > firingDistance) {
		return;
	}
	SCH->shootCannonballs(numCannons, playerPos);
}

void EnemyShipInputHandler::handleCannonAim() {
    // Set the manual aim flag to true
    SCH->setAimTowardsMouse(true);

	// Set the firing side of the ship based on the player's position
    // Calculate vector from first to second sprite
    sf::Vector2f directionVector = sprite.getPosition() - playerPos;

    // Calculate angle to horizontal
    float angleToHorizontal = atan2(directionVector.y, directionVector.x);

    // Adjust angle by first sprite's rotation
    float adjustedAngle = angleToHorizontal - sprite.getRotation() - 90.f * 3.14159265359f / 180.f;

    std::cout << "Adjusted angle: " << adjustedAngle << std::endl;

    // Normalize the angle to the range [0, 2*PI]
    if (adjustedAngle < 0) adjustedAngle += 2 * 3.14159265359f;

    // Determine side based on the adjusted angle
    if (adjustedAngle > 3.14159265359f) getCannonHandler()->setFiringSide(FiringSide::Port);
	else getCannonHandler()->setFiringSide(FiringSide::Starboard);
}

void EnemyShipInputHandler::handleAnchorDrop() {
	//...
}