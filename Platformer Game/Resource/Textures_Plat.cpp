#include "Textures_Plat.h"
#include <iostream>

using namespace PlatformerGame;

Textures::Textures() {
	sf::Texture player;
	player.loadFromFile("PlatformerGameSprites/stupidnicktankdog.png");
	playerTextures.push_back(player);

}

Textures::~Textures() {};

sf::Texture& Textures::grabPlayerTexture(int index){
	return playerTextures[index];
}


