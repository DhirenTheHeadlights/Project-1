#include "World_DG.h"

using namespace DhruvGame;

void World::createWorld() {
	// Setup

	bird.initializeBird();
	gameOver.initializegameOver();
	view.setCenter(bird.getSprite().getPosition());
	view.setSize(window->getSize().x, window->getSize().y);
	createPipes();
}

void World::gameLoop() {
	// Here, the code is called every frame. All you have to do is
	// display objects and update them. Do not fiddle around with the window
	// other than the window->draw(object) function.
	handleInput();
	update();
	render();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}
}

void World::handleInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		bird.getSprite().move(0, -3.f);
	
	}
        
}

void World::reset() {
	bird.pauseBird();
	
	gameOver.draw(window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		if (gameOver.getRestartButton().getGlobalBounds().contains(mousePosition)) {
			bird.getSprite().setPosition(-200, 300);
			bird.setVelocity(bird.getOGVelocity());
			bird.setGravity(bird.getOGGravity());
			pipes.clear();
			initial = 100;
			createPipes();
			isGameOver = false;
			gameLoop();
		}
		else if (gameOver.getQuitButton().getGlobalBounds().contains(mousePosition)) {
			window->close();
		}	
	}
	
}

void World::render() {
	bird.draw(window);
	score.draw(window);
	
	for (auto& pipe : pipes) {
		pipe.draw(window);
	}
}

void World::update() {
	bird.moveBird();
	window->setView(view);
	for (auto& pipes : pipes) {
		if (bird.getLowerBB().getGlobalBounds().intersects(pipes.getUBB().getGlobalBounds()) ||
			bird.getMidBB().getGlobalBounds().intersects(pipes.getUBB().getGlobalBounds()) ||
			bird.getUpperBB().getGlobalBounds().intersects(pipes.getUBB().getGlobalBounds()) || 
			bird.getLowerBB().getGlobalBounds().intersects(pipes.getLBB().getGlobalBounds()) ||
			bird.getMidBB().getGlobalBounds().intersects(pipes.getLBB().getGlobalBounds()) ||
			bird.getUpperBB().getGlobalBounds().intersects(pipes.getLBB().getGlobalBounds())) {
			isGameOver = true;
			reset();
		}
	}
}

void World::createPipes() {
	for (int i = 0; i < 3000; ++i) {
		Pipe pipe;
		pipe.initializePipe();
		pipe.setPosition(sf::Vector2f(initial, 0));
		initial += pipe.getDistance();
		pipes.push_back(pipe);

	}
}	