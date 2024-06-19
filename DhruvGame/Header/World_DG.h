#pragma once

/// This class is used to represent the world in the game

#include <SFML/Graphics.hpp>
#include <vector>

namespace DhruvGame {
	class World {
	public:
		World(sf::RenderWindow* window) : window(window) {};
		void createWorld();
		void gameLoop();
	private:
		sf::RenderWindow* window = nullptr;
		
		struct Tower {
			sf::CircleShape shape;
			float range;
			float fireRate;
			sf::Clock fireClock;
		};

		struct Enemy {
			sf::RectangleShape shape;
			float speed;
		};

		std::vector<Tower> towers;
		std::vector<Enemy> enemies;

		void updateEnemies();
		void checkCollisions();
	};
}