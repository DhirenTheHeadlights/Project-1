#include "World_DG.h"

using namespace DhruvGame;

void World::createWorld() {
	// Setup
	
	bird.initializeBird();
	view.setCenter(bird.getSprite().getPosition());
	view.setSize(window->getSize().x, window->getSize().y);

	float lastPositionx = 0;
	for (int i = 0; i < 30; ++i) {
		Pipe pipe;
		pipe.initializePipe();
		pipe.setPosition(sf::Vector2f(lastPositionx * 1/3 * i, 10 + rand() % 30));
		lastPositionx = pipe.getDistance();
		pipes.push_back(pipe);
	}
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
	bird.getSprite().setPosition(100, 440);
	bird.setVelocity(3);
	isGameOver = false;
}

void World::render() {
	bird.draw(window, true);
	view.setCenter(bird.getSprite().getPosition().x, bird.getSprite().getPosition().y);	
	for (auto& pipe : pipes) {
		pipe.draw(window);
	}
}

void World::update() {
	bird.moveBird();
	window->setView(view);
	for (auto& pipes : pipes) {
		if (bird.getLowerBB().getGlobalBounds().intersects(pipes.getBB().getGlobalBounds()) ||
			bird.getMidBB().getGlobalBounds().intersects(pipes.getBB().getGlobalBounds()) ||
			bird.getUpperBB().getGlobalBounds().intersects(pipes.getBB().getGlobalBounds())) {
			isGameOver = true;
			reset();
		}
	}
}