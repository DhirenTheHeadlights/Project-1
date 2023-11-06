#include "Arena.h"

Arena::Arena(sf::RenderWindow& window) : window(window), hashmap() {
}

void Arena::createArena() {
    assignTargetsToHashmap();
    updateTargets();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        checkCollision();
        //std::cout << "Mouse clicked!" << std::endl;
    }
    window.display();
}

void Arena::updateTargets() {
    if (activeTargets.size() < maxTargets) {
		drawTarget();
	}
}

void Arena::drawTarget() {
    Target* target = createTarget();
    window.draw(target->getShape());
}

Target* Arena::createTarget() {
    sf::Vector2f pos = sf::Vector2f(std::rand() % window.getSize().x, std::rand() % window.getSize().y);
    Target* target = new Target(pos, size);
    activeTargets.push_back(target);
    return target;
}

void Arena::assignTargetsToHashmap() {
    for (Target* target : activeTargets) {
		hashmap.addTarget(target);
	}
}

void Arena::checkCollision() {
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    Target* target = hashmap.getTargetAtCollision(mousePos, size);
    if (target != nullptr) {
        target->deActivate();
        std::cout << "Target hit!" << std::endl;
    }
    else {
		std::cout << "Target missed!" << std::endl;
	}
    
    for (int i = 0; i < activeTargets.size(); i++) {
        if (!activeTargets[i]->isActive()) {
			activeTargets.erase(activeTargets.begin() + i);
		}
	}
}