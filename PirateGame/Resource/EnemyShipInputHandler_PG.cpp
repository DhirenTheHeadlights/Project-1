#include "EnemyShipInputHandler_PG.h"

using namespace PirateGame;

void EnemyShipInputHandler::handleCannonFire() {
	// Only fire if the player is within firingRange
	float distance = static_cast<float>(sqrt(pow(playerPos.x - sprite.getPosition().x, 2) + pow(playerPos.y - sprite.getPosition().y, 2)));
	if (distance > firingDistance) {
		return;
	}
	SCH->shootCannonballs(numCannons, playerPos);
}

void EnemyShipInputHandler::handleCannonAim() {
    // Set the manual aim flag to true
    SCH->setFreeAim(true);

    // Calculate vector from enemy ship to player ship
    sf::Vector2f directionVector = playerPos - sprite.getPosition();

    // Calculate angle to horizontal in radians
    float angleToHorizontal = atan2(directionVector.y, directionVector.x);

    // Convert radians to degrees
    float angleInDegrees = angleToHorizontal * (180.f / 3.14159f);

    // Get the enemy ship's current rotation and adjust it to the same 0-360 degree range
    float shipRotation = fmod(sprite.getRotation(), 360.f);
    if (shipRotation < 0) shipRotation += 360.f; // Not necessary, but just in case

    // Adjust the calculated angle by subtracting the enemy ship's rotation
    float adjustedAngle = angleInDegrees - shipRotation;

    // Use fmod to normalize the adjusted angle to the range [-360, 360)
    adjustedAngle = fmod(adjustedAngle, 360.f);

    // If the result is negative, add 360 to bring it into the range [0, 360)
    if (adjustedAngle < 0) adjustedAngle += 360.f;

    // Determine side based on the adjusted angle
    if (adjustedAngle > 90.f && adjustedAngle < 270.f) getCannonHandler()->setFiringSide(FiringSide::Port);
    else getCannonHandler()->setFiringSide(FiringSide::Starboard);
}


void EnemyShipInputHandler::handleAnchorDrop() {
	//...
}