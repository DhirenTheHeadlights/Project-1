#pragma once

/// Map for pirate game

#include <unordered_map>

#include "GlobalValues_PG.h"
#include "Chunk_PG.h"

namespace PirateGame {
	class GlobalChunkHandler {
	public:
		// Public method to access the instance
		static GlobalChunkHandler& getInstance() {
			static GlobalChunkHandler instance;
			return instance;
		}

		void initializeMap() {
			// Generate the initial chunks
			lastChunk = std::make_shared<Chunk>(std::pair<int, int>(0, 0), chunkSize, cellSize);
			generateSurroundingChunks(lastChunk.get());
		}

		void generateRegionCoords(const std::pair<int, int>& chunkCoord) {

		}

		void updateChunks(const sf::Vector2f position, bool debug = false) {
			std::shared_ptr<Chunk> currentChunk = getChunkAtPosition(position);
			// Check if the player has moved to a new chunk
			if (currentChunk->getChunkCoord() != lastChunk->getChunkCoord()) {
				generateSurroundingChunks(currentChunk.get());
				deleteChunksOutOfRange(currentChunk.get());
			}
			lastChunk = currentChunk;

			// Draw the grid for all chunks
			if (debug) {
				for (auto& chunk : chunks) {
					chunk->getMap()->drawGrid(*GlobalValues::getInstance().getWindow());
				}
			}

		}


		// Setters
		void setRenderDistance(int distance) { renderDistance = distance; }

		// Getters
		std::shared_ptr<Chunk> getChunkAtPosition(sf::Vector2f position) {
			for (auto& chunk : chunks) {
				if (chunk->getMap()->mapContains(position)) {
					return chunk;
				}
			}
			std::cout << "No chunk found at position: " << position.x << ", " << position.y << std::endl;
			return nullptr;
		}
		std::vector<std::shared_ptr<Chunk>> getAllChunks() {
			return chunks;
		}

	private:
		// Private Constructor
		GlobalChunkHandler() {};

		// Delete the copy constructor and assignment operator
		GlobalChunkHandler(GlobalChunkHandler const&) = delete;
		GlobalChunkHandler& operator=(GlobalChunkHandler const&) = delete;


		// Vector to store 'chunks'
		std::vector<std::shared_ptr<Chunk>> chunks;

		// Store the last chunk the player was in
		std::shared_ptr<Chunk> lastChunk;

		void generateSurroundingChunks(Chunk* currentChunk) {
			for (int dx = -renderDistance; dx <= renderDistance; ++dx) {
				for (int dy = -renderDistance; dy <= renderDistance; ++dy) {
					std::pair<int, int> chunkCoord = { currentChunk->getChunkCoord().first + dx, currentChunk->getChunkCoord().second + dy};
					generateChunk(chunkCoord);
				}
			}
		};

		void generateChunk(const std::pair<int, int>& chunkCoord) {
			// Check if the chunk is already generated
			for (auto& chunk : chunks) {
				if (chunk->getChunkCoord() == chunkCoord) {
					return;
				}
			}
			// Generate the chunk
			std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>(chunkCoord, chunkSize, cellSize);
			chunks.push_back(newChunk);
		}
		
		void deleteChunksOutOfRange(Chunk* currentChunk) {
			std::vector<Chunk*> chunksToDelete;
			for (auto& chunk : chunks) {
				if (abs(chunk->getChunkCoord().first - currentChunk->getChunkCoord().first) > renderDistance || 
					abs(chunk->getChunkCoord().second - currentChunk->getChunkCoord().second) > renderDistance) {
					chunksToDelete.push_back(chunk.get());
				}
			}

			for (auto& chunk : chunksToDelete) {
				deleteChunk(chunk);
			}
		}

		void deleteChunk(Chunk* chunk) {
			// Check if the chunk exists, then erase.
			std::erase_if(chunks, [&](const std::shared_ptr<Chunk>& c) { return c.get() == chunk; });
		}

		// Values
		sf::Vector2f chunkSize = sf::Vector2f(5000.f, 5000.f);
		int cellSize = 100;
		int renderDistance = 2;
		int regionRenderDistance = 10;
	};
}
