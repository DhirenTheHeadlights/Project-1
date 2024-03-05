#include "HUD_Plat.h"

using namespace PlatformerGame;

HealthBar::HealthBar(float maxHealth, sf::Vector2f position, sf::Vector2f size) {
	this->maxHealth = maxHealth;
	this->health = maxHealth;

	healthBar.setSize(size);
	healthBar.setPosition(position);
	healthBar.setFillColor(sf::Color::Green);

	healthBarBackground = healthBar;
	healthBarBackground.setFillColor(sf::Color(100, 100, 100));
}

void HealthBar::setHealth(float health) {
	this->health = std::max(0.0f, std::min(health, maxHealth));
}

void HealthBar::setMaxHealth(float maxHealth) {
	this->maxHealth = maxHealth;
}

void HealthBar::update() {
	float percentage = health / maxHealth;
	sf::Vector2f size = healthBarBackground.getSize();
	size.x *= percentage;
	healthBar.setSize(size);
}

void HealthBar::draw(sf::RenderWindow& window) {
	window.draw(healthBarBackground);
	window.draw(healthBar);
}