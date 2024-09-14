#pragma once

/// Map for pirate game

#include "Chunk_PG.h"
#include "RegionHandler_PG.h"

namespace PirateGame {
	class GlobalChunkHandler {
	public:
		GlobalChunkHandler() {};

		void initializeMap() {
			// Generate the initial chunks
			Chunk initialChunk(std::pair<int, int>(0, 0), chunkSize, cellSize);
			generateSurroundingChunks(initialChunk);
			updateMapBounds();
		}

		static void generateRegionCoords(const std::pair<int, int>& chunkCoord) {

		}

		void updateChunks(sf::RenderWindow* window, const sf::Vector2f position, bool debug = false) {
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
		

		// Setters
		void setRenderDistance(int distance) { renderDistance = distance; }

		// Getters
		Chunk& getChunkAtPosition(const sf::Vector2f position) {
			for (auto& chunk : chunks) {
				if (chunk.getMap()->mapContains(position)) {
					return chunk;
				}
			}
			std::cout << "No chunk found at position: " << position.x << ", " << position.y << std::endl;
			static Chunk defaultChunk(std::pair<int, int>(0, 0), chunkSize, cellSize);
			return defaultChunk;
		}
		std::vector<Chunk>& getAllChunks() {
			return chunks;
		}
		RegionHandler& getRegionHandler() { return regionHandler; }
		sf::FloatRect getMapBounds() const { return mapBounds; }

	private:
		// Vector to store 'chunks'
		std::vector<Chunk> chunks;

		// Store the last chunk the player was in
		std::pair<int, int> lastChunkCoord = { 0, 0 };

		void generateSurroundingChunks(const Chunk& currentChunk) {
			for (int dx = -renderDistance; dx <= renderDistance; ++dx) {
				for (int dy = -renderDistance; dy <= renderDistance; ++dy) {
					std::pair<int, int> chunkCoord = { currentChunk.getChunkCoord().first + dx, currentChunk.getChunkCoord().second + dy};
					generateChunk(chunkCoord);
				}
			}
		};

		void generateChunk(const std::pair<int, int>& chunkCoord) {
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
		
		void deleteChunksOutOfRange(Chunk& currentChunk) {
			/*std::vector<Chunk> chunksToDelete;
			for (auto& chunk : chunks) {
				if (abs(chunk.getChunkCoord().first - currentChunk.getChunkCoord().first) > renderDistance || 
					abs(chunk.getChunkCoord().second - currentChunk.getChunkCoord().second) > renderDistance) {
					chunksToDelete.push_back(chunk);
				}
			}

			for (auto& chunk : chunksToDelete) {
				deleteChunk(chunk);
			}*/
		}

		void deleteChunk(Chunk& chunk) {
			// Check if the chunk exists, then erase.
			std::erase_if(chunks, [&chunk](const Chunk& c) { return c.getChunkCoord() == chunk.getChunkCoord(); });
		}

		void updateMapBounds() {
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


		// Values
		RegionHandler regionHandler;
		sf::Vector2f chunkSize = sf::Vector2f(5000.f, 5000.f);
		sf::FloatRect mapBounds = sf::FloatRect(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), 0.f, 0.f);
		int cellSize = 100;
		int renderDistance = 2;
		int regionRenderDistance = 10;
		std::vector<float> regionScaling = { 0.5f, 0.5f, 0.5f, 0.5f };
	};
}
