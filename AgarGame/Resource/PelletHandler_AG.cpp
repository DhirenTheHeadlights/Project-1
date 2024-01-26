#include "PelletHandler_AG.h"

PelletHandler::PelletHandler(sf::RenderWindow& window) {
}

void PelletHandler::drawPellets(sf::RenderWindow& window, Map& map, const int numPellets) {
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
    //std::cout << "Number of active pellets: " << activePellets.size() << std::endl;
}

void PelletHandler::eraseIfInactive() {
    activePellets.erase( // Remove all inactive pellets from the activePellets vector
        std::remove_if(activePellets.begin(), activePellets.end(), [](const std::shared_ptr<Pellet>& pelletPtr) {
            return !pelletPtr->isActive(); }), activePellets.end());
    //std::cout << "erased inactive pellets" << std::endl;
}

std::vector<std::shared_ptr<Pellet>> PelletHandler::getActivePellets() {
    return activePellets;
}
