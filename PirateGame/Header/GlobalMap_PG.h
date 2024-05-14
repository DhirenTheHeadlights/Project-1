#pragma once

/// Map for pirate game

#include <unordered_map>

#include "GlobalValues_PG.h"
#include "Map.h"

namespace PirateGame {
	class GlobalMap {
	public:
		// Public method to access the instance
		static GlobalMap& getInstance() {
			static GlobalMap instance;
			return instance;
		}

		void initializeMap() {
			// Generate the initial chunks
			auto currentChunk = calculateCurrentChunk(sf::Vector2f(0.f, 0.f));
			generateSurroundingChunks(currentChunk);
		}

		void updateChunks(const sf::Vector2f position) {
			auto currentChunk = calculateCurrentChunk(position);
			// Check if the player has moved to a new chunk
			if (currentChunk != lastChunk) {
				generateSurroundingChunks(currentChunk);
				deleteChunksOutOfRange(currentChunk);
			}
			lastChunk = currentChunk;

			// Draw the grid for all chunks
			for (auto& chunk : chunks) {
				//chunk.second->drawGrid(*GlobalValues::getInstance().getWindow());
			}
		}


		// Setters
		void setRenderDistance(int distance) { renderDistance = distance; }

		// Getters
		Map* getMapAtCurrentChunk(sf::Vector2f position) {
			if (chunks.find(calculateCurrentChunk(position)) != chunks.end()) {
				return chunks[calculateCurrentChunk(position)].get();
			}
			else {
				std::cout << "Chunk not found at position: " << position.x << ", " << position.y << std::endl;
				return nullptr;
			}
		}
		std::vector<Map*> getAllChunks() {
			std::vector<Map*> allChunks;
			for (auto& chunk : chunks) {
				allChunks.push_back(chunk.second.get());
			}
			return allChunks;
		}

	private:
		// Private Constructor
		GlobalMap() {};

		// Delete the copy constructor and assignment operator
		GlobalMap(GlobalMap const&) = delete;
		GlobalMap& operator=(GlobalMap const&) = delete;

		// Struct for comparing pairs
		struct pair_hash {
			template <class T1, class T2>
			std::size_t operator () (const std::pair<T1, T2>& pair) const {
				auto hash1 = std::hash<T1>{}(pair.first);
				auto hash2 = std::hash<T2>{}(pair.second);
				return hash1 ^ hash2;
			}
		};

		// Unordered Map to store 'chunks'
		std::unordered_map<std::pair<int, int>, std::unique_ptr<Map>, pair_hash> chunks;

		// Store the last chunk the player was in
		std::pair<int, int> lastChunk = { 0, 0 };

		std::pair<int, int> calculateCurrentChunk(const sf::Vector2f& position) const {
			// Calculate the chunk indices based on the player's position and the cell size
			int xChunk = static_cast<int>(position.x / (cellSize * chunkSize.x));
			int yChunk = static_cast<int>(position.y / (cellSize * chunkSize.y));
			return { xChunk, yChunk };
		}

		void generateSurroundingChunks(const std::pair<int, int>& currentChunk) {
			for (int dx = -renderDistance; dx <= renderDistance; ++dx) {
				for (int dy = -renderDistance; dy <= renderDistance; ++dy) {
					std::pair<int, int> chunkCoord = { currentChunk.first + dx, currentChunk.second + dy };
					generateChunk(chunkCoord);
				}
			}
		};

		void generateChunk(const std::pair<int, int>& chunkCoord) {
			// Check if the chunk is already generated
			if (chunks.find(chunkCoord) == chunks.end()) {
				// Generate the chunk
				auto newChunk = std::make_unique<Map>();
				newChunk->grid(static_cast<int>(chunkSize.x), static_cast<int>(chunkSize.y), cellSize, sf::Vector2f(chunkCoord.first * chunkSize.x, chunkCoord.second * chunkSize.y));
				chunks[chunkCoord] = std::move(newChunk);
			}
		}
		
		void deleteChunksOutOfRange(const std::pair<int, int>& currentChunk) {
			std::vector<std::pair<int, int>> chunksToDelete;
			for (auto& chunk : chunks) {
				if (abs(chunk.first.first - currentChunk.first) > renderDistance || abs(chunk.first.second - currentChunk.second) > renderDistance) {
					chunksToDelete.push_back(chunk.first);
				}
			}

			for (auto& chunk : chunksToDelete) {
				deleteChunk(chunk);
			}
		}

		void deleteChunk(const std::pair<int, int>& chunkCoord) {
			// Check if the chunk exists
			if (chunks.find(chunkCoord) != chunks.end()) {
				chunks.erase(chunkCoord);
			}
		}

		// Values
		sf::Vector2f chunkSize = sf::Vector2f(5000.f, 5000.f);
		int cellSize = 100;
		int renderDistance = 2;
	};
}
