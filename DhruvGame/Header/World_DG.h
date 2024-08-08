#pragma once

/// This class is used to represent the world in the game

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Bird_DG.h"
#include "Pipe_DG.h"
#include "gameOver_DG.h"

namespace DhruvGame {
	class World {
	public:
		World(sf::RenderWindow* window) : window(window) {}
		float initial = 400;
	
		void createWorld();
		void gameLoop();
		void createPipes();
	
	private:
		sf::RenderWindow* window;
		sf::View view;
		sf::Clock clock;
		std::vector<Pipe> pipes;
		Bird bird;
		gameOver gameOver;
		bool isGameOver = false;
		void handleInput();
		void update();
		void render();
		void reset();
	};
}
