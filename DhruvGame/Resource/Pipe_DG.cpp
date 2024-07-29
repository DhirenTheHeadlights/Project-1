#include "Pipe_DG.h"
#include <iostream>
using namespace DhruvGame;

void Pipe::initializePipe() {
	static sf::Texture texture;
	if (!texture.loadFromFile("DhruvGame/Sprites/pipe.png")) {
		std::cerr << "Failed to load pipe texture" << std::endl;
	}
	else {
		sprite.setTexture(texture);
		boundingBox.setSize(sf::Vector2f(3 * scaleSizex, 64 * scaleSizey));

	}
	//texture.loadFromFile("DhruvGame/Sprites/pipe.png");
	//sprite.setTexture(texture);
	
}

