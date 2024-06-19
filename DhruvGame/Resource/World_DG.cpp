#include "World_DG.h"

using namespace DhruvGame;


void World::createWorld() {
	Tower tower;
	tower.shape.setRadius(20.f);
	tower.fireRate = 1.f;
	tower.shape.setFillColor(sf::Color::Blue);
	tower.shape.setPosition(100.f, 100.f);
	tower.range = 150.f;
	towers.push_back(tower);

	Enemy enemy;
	enemy.shape.setSize(sf::Vector2f(100.f, 100.f);
	enemy.speed = 50.f;
}

void World::gameLoop() {
	// Here, the code is called every frame. All you have to do is
	// display objects and update them. Do not fiddle around with the window
	// other than the window->draw(object) function.
	updateEnemies();
}

float World::function(float x) {
	return x * x;
}

