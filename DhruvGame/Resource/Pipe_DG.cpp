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
		lowerBB.setSize(sf::Vector2f(3 * scaleSizex, 64 * scaleSizey));
		upperBB.setSize(sf::Vector2f(3 * scaleSizex, 64 * scaleSizey));
	}
}

void Pipe::setPosition(sf::Vector2f position) {

	// Grab a random number of pixels to be in between the pipe
	float gap = 200 + rand() % 200;
	upper.setPosition(sf::Vector2f(position.x + gap, 0));
	lower.setScale(scaleSizex, -scaleSizey);
	lower.setPosition(sf::Vector2f(position.x + gap, position.y + 1080));


	lowerBB.setPosition(lower.getPosition());
	upperBB.setPosition(upper.getPosition());
}
//position.y + upper.getGlobalBounds().height + gap