#include "ChunkHandler_PG.h"

using namespace PirateGame;

// Values
int cellSize = 100;
int renderDistance = 2;
int regionRenderDistance = 10;
std::vector<float> regionScaling = { 0.5f, 0.5f, 0.5f, 0.5f };
std::pair<int, int> lastChunkCoord = { 0, 0 };

RegionHandler ChunkHandler::regionHandler;
std::vector<Chunk> ChunkHandler::chunks;
sf::FloatRect ChunkHandler::mapBounds = sf::FloatRect(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), 0.f, 0.f);
sf::Vector2f ChunkHandler::chunkSize = sf::Vector2f(5000.f, 5000.f);

void ChunkHandler::initializeMap() {
// Generate the initial chunks
	Chunk initialChunk(std::pair<int, int>(0, 0), chunkSize, cellSize);
	generateSurroundingChunks(initialChunk);
	updateMapBounds();
}

void ChunkHandler::updateChunks(sf::RenderWindow* window, const sf::Vector2f position, bool debug) {
	Chunk& currentChunk = getChunkAtPosition(position);

	// Check if the player has moved to a new chunk
	if (currentChunk.getChunkCoord() != lastChunkCoord) {
		generateSurroundingChunks(currentChunk);
		deleteChunksOutOfRange(currentChunk);

		// Update the map bounds
		updateMapBounds();

		lastChunkCoord = currentChunk.getChunkCoord();
	}

	// Draw the grid for all chunks
	if (debug) {
		for (auto& chunk : chunks) {
			chunk.getMap()->drawGrid(*window);
		}
	}
}

void ChunkHandler::generateSurroundingChunks(const Chunk& currentChunk) {
	for (int dx = -renderDistance; dx <= renderDistance; ++dx) {
		for (int dy = -renderDistance; dy <= renderDistance; ++dy) {
			std::pair<int, int> chunkCoord = { currentChunk.getChunkCoord().first + dx, currentChunk.getChunkCoord().second + dy };
			generateChunk(chunkCoord);
		}
	}
};

void ChunkHandler::generateChunk(const std::pair<int, int>& chunkCoord) {
	// Check if the chunk is already generated
	for (auto& chunk : chunks) {
		if (chunk.getChunkCoord() == chunkCoord) {
			return;
		}
	}
	// Generate the chunk
	Chunk newChunk(chunkCoord, chunkSize, cellSize);
	newChunk.setRegionType(regionHandler.generateRegionType(sf::Vector2f(static_cast<float>(chunkCoord.first), static_cast<float>(chunkCoord.second))));
	chunks.push_back(std::move(newChunk));
}

void ChunkHandler::deleteChunksOutOfRange(Chunk& currentChunk) {
	//std::vector<Chunk> chunksToDelete;
	//for (auto& chunk : chunks) {
	//	if (abs(chunk.getChunkCoord().first - currentChunk.getChunkCoord().first) > renderDistance ||
	//		abs(chunk.getChunkCoord().second - currentChunk.getChunkCoord().second) > renderDistance) {
	//		chunksToDelete.push_back(chunk);

	//		//for (std::shared_ptr<EnemyShip>& ship : chunk.getEnemyShips()) {
	//		//	ship->setDead(true);
	//		//}
	//	}
	//}

	//for (auto& chunk : chunksToDelete) {
	//	deleteChunk(chunk);
	//}
}

void ChunkHandler::deleteChunk(Chunk& chunk) {
	// Check if the chunk exists, then erase.
	std::erase_if(chunks, [&chunk](const Chunk& c) { return c.getChunkCoord() == chunk.getChunkCoord(); });
}

void ChunkHandler::updateMapBounds() {
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();

	for (const auto& chunk : chunks) {
		float chunkLeft = chunk.getChunkCoord().first * chunkSize.x;
		float chunkTop = chunk.getChunkCoord().second * chunkSize.y;
		float chunkRight = chunkLeft + chunkSize.x;
		float chunkBottom = chunkTop + chunkSize.y;

		minX = std::min(minX, chunkLeft);
		maxX = std::max(maxX, chunkRight);
		minY = std::min(minY, chunkTop);
		maxY = std::max(maxY, chunkBottom);
	}

	mapBounds.left = minX;
	mapBounds.top = minY;
	mapBounds.width = maxX - minX;
	mapBounds.height = maxY - minY;
}

//Setters
void ChunkHandler::setRenderDistance(int distance) { renderDistance = distance; }

// Getters
Chunk& ChunkHandler::getChunkAtPosition(const sf::Vector2f position) {
	for (auto& chunk : chunks) {
		if (chunk.getMap()->mapContains(position)) {
			return chunk;
		}
	}
	std::cout << "No chunk found at position: " << position.x << ", " << position.y << std::endl;
	static Chunk defaultChunk(std::pair<int, int>(0, 0), chunkSize, cellSize);
	return defaultChunk;
}
std::vector<Chunk>& ChunkHandler::getAllChunks() {
	return chunks;
}

RegionHandler& ChunkHandler::getRegionHandler() { return regionHandler; }

sf::FloatRect ChunkHandler::getMapBounds() { return mapBounds; }


