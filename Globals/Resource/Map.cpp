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

// This function uses a poisson disc sampling algorithm to generate a set of points
// that are at least a certain distance apart from each other. This function should be
// called once to generate the points.
std::vector<sf::Vector2f> Map::getRandomPositions(float minDistance, int numPoints) {
    const int k = 30; // attempts before giving up on finding a new point
    const float pi = 3.14159265358979323846f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::vector<sf::Vector2f> samplePoints;
    std::vector<sf::Vector2f> activeList;

    // Initialize with the first point
    sf::Vector2f initialPoint(dis(gen) * len, dis(gen) * height);
    activeList.push_back(initialPoint);
    samplePoints.push_back(initialPoint);

    while (!activeList.empty()) {
        std::uniform_int_distribution<> dist(0, activeList.size() - 1);
        int randIndex = dist(gen);
        sf::Vector2f point = activeList[randIndex];
        bool found = false;

        for (int i = 0; i < k; ++i) {
            float angle = dis(gen) * 2 * pi;
            float radius = minDistance * (1 + dis(gen));
            sf::Vector2f newPoint(point.x + radius * cos(angle), point.y + radius * sin(angle));

            // Check bounds and minimum distance
            if (newPoint.x >= 0 && newPoint.x <= len && newPoint.y >= 0 && newPoint.y <= height) {
                bool tooClose = false;
                for (const sf::Vector2f& existingPoint : samplePoints) {
                    float distance = std::hypot(newPoint.x - existingPoint.x, newPoint.y - existingPoint.y);
                    if (distance < minDistance) {
                        tooClose = true;
                        break;
                    }
                }
                if (!tooClose) {
                    activeList.push_back(newPoint);
                    samplePoints.push_back(newPoint);
                    found = true;
                    break; // Break from the attempts loop
                }
            }
        }

        if (!found) {
            // If no point is found after k attempts, remove the current point from the active list
            activeList.erase(activeList.begin() + randIndex);
        }
    }

    return samplePoints; // Return all generated points
}


// Draw the grid
void Map::drawGrid(sf::RenderWindow& window) {
    sf::RectangleShape line(sf::Vector2f(static_cast<float>(getLength()), 1.f)); // Horizontal line
    line.setFillColor(sf::Color::Red);
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