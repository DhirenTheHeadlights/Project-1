#include "gameOver_DG.h"


using namespace DhruvGame;

void gameOver::initializegameOver() {
	static sf::Texture texture;
	texture.loadFromFile("DhruvGame/Sprites/gameOverMenu.png");
	sprite.setTexture(texture);
	sprite.setScale(scaleSize);
	sprite.setPosition(500, 500);
	restartButton.setSize(sf::Vector2f(30, 10));
	restartButton.setScale(scaleSize);
	restartButton.setPosition(gameOverPosition.x + 1 * scaleSize.x, gameOverPosition.y + 1 * scaleSize.y);
	quitButton.setSize(sf::Vector2f(30, 10));
	quitButton.setScale(scaleSize);
	quitButton.setPosition(gameOverPosition.x + 1 * scaleSize.x, gameOverPosition.y + 22 * scaleSize.y);
}

sf::Vector2f gameOver::getCenter(sf::RenderWindow* window) {
	return sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2);
}