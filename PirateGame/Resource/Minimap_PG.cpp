#include "Minimap_PG.h"

using namespace PirateGame;

void Minimap::draw() {
    sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

    window->draw(minimap);
    window->draw(minimapSprite);

    // Iterate only through visible landmasses
    for (size_t i = 0; i < visibleLandmassRects.size(); ++i) {
        window->draw(visibleLandmassRects[i]);
    }

    // Iterate only through visible ships
    for (size_t i = 0; i < visibleShipRects.size(); ++i) {
		window->draw(visibleShipRects[i]);
	}

    window->draw(shipIconSprite);
}

void Minimap::update() {
    sf::Vector2f shipPos = ship->getSprite().getPosition();
    sf::Vector2f minimapCenter = sf::Vector2f(minimapSprite.getPosition().x + minimapSprite.getGlobalBounds().width / 2, minimapSprite.getPosition().y + minimapSprite.getGlobalBounds().height / 2);

    minimapSprite.setPosition(position);

    // Define the visibility radius around the ship for the minimap
    float visibilityRadius = 10000.0f;

    shipIconSprite.setPosition(minimapCenter);
    shipIconSprite.setOrigin(shipIconSprite.getLocalBounds().width / 2, shipIconSprite.getLocalBounds().height / 2);
    shipIconSprite.setRotation(ship->getSprite().getRotation());

    // Clear the previous visible landmasses
    visibleLandmassRects.clear();

    float minimapRadius = minimapSprite.getGlobalBounds().width / 2;

    for (size_t i = 0; i < landmasses.size(); ++i) {
        sf::Vector2f landmassPos = landmasses[i]->getSprite().getPosition() + // Adjust for the center of the landmass
            sf::Vector2f(landmasses[i]->getSprite().getGlobalBounds().width / 2, landmasses[i]->getSprite().getGlobalBounds().height / 2);

        // Calculate distance from the ship to the landmass
        float distance = vm::magnitude(shipPos - landmassPos);

        if (distance <= visibilityRadius) {
            sf::RectangleShape landmassRect;

            // Adjust position relative to the ship's position on the minimap
            sf::Vector2f scaledPos = (landmassPos - shipPos) * (minimapSprite.getGlobalBounds().height / 2 / visibilityRadius);
            sf::Vector2f relativePos = minimapCenter + scaledPos;

            // Ensure landmassRect is within the minimap's bounds
            if (sqrt(pow(relativePos.x - minimapCenter.x, 2) + pow(relativePos.y - minimapCenter.y, 2)) <= minimapRadius) {
                landmassRect.setPosition(relativePos);

                // Set landmass type properties
                if (landmasses[i]->getType() == LandMassType::Island) {
                    landmassRect.setFillColor(sf::Color::Green);
                    landmassRect.setSize(islandIconSize);
                }
                else if (landmasses[i]->getType() == LandMassType::Rock) {
                    landmassRect.setFillColor(sf::Color::Black);
                    landmassRect.setSize(rockIconSize);
                }
                else if (landmasses[i]->getType() == LandMassType::Shipwreck) {
                    landmassRect.setFillColor(sf::Color::Blue);
                }

                visibleLandmassRects.push_back(landmassRect);
            }
        }
    }

    // Clear visible ship rects
    visibleShipRects.clear();

    for (size_t i = 0; i < ships.size(); ++i) {
		sf::Vector2f shipPos = ships[i]->getSprite().getPosition();
        float distance = vm::magnitude(shipPos - ship->getSprite().getPosition());

        if (distance <= visibilityRadius) {
			sf::RectangleShape shipRect;
			sf::Vector2f scaledPos = (shipPos - ship->getSprite().getPosition()) * (minimapSprite.getGlobalBounds().height / 2 / visibilityRadius);
			sf::Vector2f relativePos = minimapCenter + scaledPos;

            if (sqrt(pow(relativePos.x - minimapCenter.x, 2) + pow(relativePos.y - minimapCenter.y, 2)) <= minimapRadius) {
				shipRect.setPosition(relativePos);
				shipRect.setFillColor(sf::Color::Red);
				shipRect.setSize(shipRectSize);

				visibleShipRects.push_back(shipRect);
			}
		}
	}

}
