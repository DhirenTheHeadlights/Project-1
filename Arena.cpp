#include "Arena.h"

Arena::Arena(sf::RenderWindow& window) : window(window), hashmap() {
}

void Arena::createArena() {
    updateTargets();
    checkCollision();
    window.display();
}

void Arena::updateTargets() {
    if (activeTargets.size() < maxTargets) {
		drawTargets();
	}
}

void Arena::drawTargets() {
    for (int i = 0; i < maxTargets; i++) {
        sf::Vector2f pos = sf::Vector2f(std::rand() % desktop.width, std::rand() % desktop.height);
        Target* target = new Target(pos, size);
        activeTargets.push_back(target);
    }

    for (Target* target : activeTargets) {
        hashmap.addTarget(target);
    }
}

void Arena::checkCollision() {
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    Target* target = hashmap.getTargetAtCollision(mousePos);
    target->deActivate();
    
    for (int i = 0; i < activeTargets.size(); i++) {
        if (!activeTargets[i]->isActive()) {
			activeTargets.erase(activeTargets.begin() + i);
		}
	}
}