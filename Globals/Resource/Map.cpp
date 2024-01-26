#include "Map.h"

// Constructor
void Map::grid(int boardLength, int boardHeight, int cellSize) {
    rows = boardHeight / cellSize;
    cols = boardLength / cellSize;

    // Set variables
    this->cellSize = cellSize;
    this->len = boardLength;
    this->height = boardHeight;
}

// Setters
void Map::setCellSize(int cellSize) {
	this->cellSize = cellSize;
}


// Getters
int Map::getCellSize() const {
    return cellSize;
}

int Map::getLength() const {
    return rows * cellSize;
}

std::pair<int, int> Map::getGridCoordinates(float x, float y) const {
    return { static_cast<int>(x / cellSize), static_cast<int>(y / cellSize) };
}

std::optional<sf::Vector2f> Map::getRandomPosition(float spacing) {
    // Constants
    const int k = 30; // Number of attempts to find a suitable point
    const float pi = 3.14159265358979323846;

    // Statics
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    // Initialize the first point and active list if they are empty
    if (samplePoints.empty()) {
        sf::Vector2f initialPoint(dis(gen) * len, dis(gen) * height);
        activeList.push_back(initialPoint);
        samplePoints.push_back(initialPoint);
    }

    if (activeList.empty()) return std::nullopt;
    
    std::uniform_int_distribution<> dist(0, activeList.size() - 1);
    int randIndex = dist(gen);
    sf::Vector2f point = activeList[randIndex];

    for (int i = 0; i < k; ++i) {
        float angle = dis(gen) * 2 * pi;
        float radius = spacing * (1 + dis(gen));
        sf::Vector2f newPoint(point.x + radius * cos(angle), point.y + radius * sin(angle));
        
        // Check if the point is within bounds and far enough from existing points
        if (newPoint.x >= 0 && newPoint.x <= len && newPoint.y >= 0 && newPoint.y <= height) {
            bool tooClose = false;
            for (const sf::Vector2f& existingPoint : samplePoints) {
                float dist = std::sqrt((newPoint.x - existingPoint.x) * (newPoint.x - existingPoint.x) +
                    (newPoint.y - existingPoint.y) * (newPoint.y - existingPoint.y));
                if (dist < spacing) {
                    tooClose = true;
                    break;
                }
            }
            if (!tooClose) {
                activeList.push_back(newPoint);
                samplePoints.push_back(newPoint);
                std::cout << "Valid point found" << std::endl;
                return newPoint; // Return the new valid point
            }
        }
    }

    // Remove the point from the active list after all attempts have failed
    activeList.erase(activeList.begin() + randIndex);

    std::cout << "No valid point found" << std::endl;

    return std::nullopt;
}

// Helper function to check points


// Draw the grid
void Map::drawGrid(sf::RenderWindow& window) {
    sf::RectangleShape line(sf::Vector2f(static_cast<float>(getLength()), 1.f)); // Horizontal line
    line.setFillColor(sf::Color::Green);
    for (int i = 0; i <= rows; i++) {
        line.setPosition(0, static_cast<float>(i * cellSize));
        window.draw(line);
    }
    line.setSize(sf::Vector2f(1.f, static_cast<float>(getLength()))); // Vertical line
    for (int i = 0; i <= cols; i++) {
        line.setPosition(static_cast<float>(i * cellSize), 0);
        window.draw(line);
    }
}