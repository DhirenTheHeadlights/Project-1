#pragma once

/// This class is used to represent the world in the game

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Bird_DG.h"
#include "Pipe_DG.h"

namespace DhruvGame {
	class World {
	public:
		World(sf::RenderWindow* window) : window(window) {}

		void createWorld();
		void gameLoop();

	private:
		sf::RenderWindow* window;
		Bird bird;
		std::vector<Pipe> pipes;
		
		float gravity = 10.f;
		sf::Clock clock;
		bool isGameOver = false;

		void handleInput();
		void update();
		void render();
		void reset();
	};
}
