#include "Pipe_DG.h"
#include <iostream>
using namespace DhruvGame;

void Pipe::initializePipe() {
	static sf::Texture texture;
	if (!texture.loadFromFile("DhruvGame/Sprites/pipe.png")) {
		std::cerr << "Failed to load pipe texture" << std::endl;
	}
	else {
		upper.setTexture(texture);
		lower.setTexture(texture);
		lowerBB.setSize(sf::Vector2f(3 * scaleSizex, -64 * scaleSizey));
		upperBB.setSize(sf::Vector2f(3 * scaleSizex, 64 * scaleSizey));
	}
}

void Pipe::setPosition(sf::Vector2f position) {
	distance = 200 + ((rand() % 500));
	float gap = 130 + rand() % 100;
	lower.setScale(scaleSizex, -scaleSizey);
	upper.setScale(scaleSizex, scaleSizey);
	upper.setPosition(sf::Vector2f(position.x, position.y));
	lower.setPosition(sf::Vector2f(position.x, position.y + (128 * scaleSizey) + gap));


	lowerBB.setPosition(position.x + 14 * scaleSizex, lower.getPosition().y);
	upperBB.setPosition(position.x + 14 * scaleSizex, upper.getPosition().y);
}

