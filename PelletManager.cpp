#include "PelletManager.h"

PelletManager::PelletManager(sf::RenderWindow& window) : window(window) {
}

void PelletManager::drawPellets(sf::RenderWindow& window, Map& map, const int numPellets) {
    if (activePellets.empty()) {
        for (int i = 0; i < numPellets; i++) {
            float x = static_cast<float>(std::rand() % map.getLength());
            float y = static_cast<float>(std::rand() % map.getLength());
            activePellets.push_back(std::make_shared<Pellet>(x, y));
        }
    }
    if (mainTime.getElapsedTime().asSeconds() > timeBetweenPelletsSpawning) {
        float x = static_cast<float>(std::rand() % map.getLength());
        float y = static_cast<float>(std::rand() % map.getLength());
        activePellets.push_back(std::make_shared<Pellet>(x, y));
        mainTime.restart();
    }
    for (const auto& pelletPtr : activePellets) {
        pelletPtr->draw(window);
    }
}

void PelletManager::eraseIfInactive() {
    activePellets.erase( // Remove all inactive pellets from the activePellets vector
        std::remove_if(activePellets.begin(), activePellets.end(), [](const std::shared_ptr<Pellet>& pelletPtr) {
            return !pelletPtr->isActive(); }), activePellets.end());
}

std::vector<std::shared_ptr<Pellet>> PelletManager::getActivePellets() {
	return activePellets;
}
