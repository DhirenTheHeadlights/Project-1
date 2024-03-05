#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace PlatformerGame {
	class HealthBar {
	public:
		HealthBar(float maxHealth, sf::Vector2f position, sf::Vector2f size);
		void setHealth(float health);
		void setMaxHealth(float maxHealth);
		void update();
		void draw(sf::RenderWindow& window);

	private:
		sf::RectangleShape healthBar;
		sf::RectangleShape healthBarBackground;
		float health = 100;
		float maxHealth = 100;
	};
}

