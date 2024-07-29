#include "World_DG.h"

using namespace DhruvGame;


World::World(sf::RenderWindow* window)
{
	this->window = window;
	
	
}

void World::createWorld() {
	// Setup
	bird.initializeBird();
	
	for (int i = 0; i < 10; ++i) {
		Pipe pipe;
		pipe.initializePipe();
		pipe.getSprite().setPosition(300 + i * 300, 0);
		pipe.getSprite().setPosition(300 + i * 300, 1080);
		pipe.getSprite().setScale(6.f, 7.f);
		pipe.getSprite().setScale(6.f, -7.f);
		pipes.push_back(pipe);
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
		bird.getSprite().move(0, -20);
		
	}
}

void World::reset() {
	bird.getSprite().setPosition(100, 440);
	bird.setVelocity(10);
	isGameOver = false;
}

void World::render() {
	window->draw(bird.getSprite());
	window->draw(bird.getLowerBB());
	window->draw(bird.getMidBB());
	window->draw(bird.getUpperBB());
	for (auto& pipe : pipes) {
		window->draw(pipe.getSprite());
	}

}

void World::update() {
	/*bird.setVelocity(bird.getVelocity() + gravity * dt);
	bird.getSprite().move(5, bird.getVelocity() * dt);*/
	sf::RectangleShape& lowerBB = bird.getLowerBB();
	sf::RectangleShape& midBB = bird.getMidBB();
	sf::RectangleShape& upperBB = bird.getUpperBB();
	
	bird.moveBird();

	for (auto& pipes : pipes) {
		if (bird.getLowerBB().getGlobalBounds().intersects(pipes.getBB().getGlobalBounds()) ||
			bird.getMidBB().getGlobalBounds().intersects(pipes.getBB().getGlobalBounds()) ||
			bird.getUpperBB().getGlobalBounds().intersects(pipes.getBB().getGlobalBounds())) {
			isGameOver = true;
			reset();
		}
	}
}