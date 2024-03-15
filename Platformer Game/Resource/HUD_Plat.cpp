#include "HUD_Plat.h"

using namespace PlatformerGame;

HUD::HUD(float maxHealth, sf::Vector2f position, sf::Vector2f size) {
	this->maxHealth = maxHealth;
	this->health = maxHealth;

	healthBar.setSize(size);
	healthBar.setPosition(position);
	healthBar.setFillColor(sf::Color::Green);

	healthBarBackground = healthBar;
	healthBarBackground.setFillColor(sf::Color(100, 100, 100));
}

void HUD::setHealth(float health) {
	this->health = std::max(0.0f, std::min(health, maxHealth));
}

void HUD::setMaxHealth(float maxHealth) {
	this->maxHealth = maxHealth;
}

void HUD::update(sf::RenderWindow& window) {
	float percentage = health / maxHealth;
	sf::Vector2f size = healthBarBackground.getSize();
	size.x *= percentage;
	healthBar.setSize(size);
	//make health bar stay at the top of the screen in relative position to the player sprite
	sf::Vector2f viewposition = window.getView().getCenter();
	healthBar.setPosition(viewposition.x - healthBar.getSize().x/2, viewposition.y - window.getSize().y / 2 + 50);
	healthBarBackground.setPosition(viewposition.x - healthBarBackground.getSize().x / 2, viewposition.y - window.getSize().y / 2 + 50);
}

void HUD::draw(sf::RenderWindow& window) {
	window.draw(healthBarBackground);
	window.draw(healthBar);
}