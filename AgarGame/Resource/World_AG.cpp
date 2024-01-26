#include "World_AG.h"

using namespace AgarGame;

World::World(sf::RenderWindow& window) : colMan(window, map), view(window.getDefaultView()), pelHan(window), debug(window), input(window), window(window) {
    map.grid(10000, 10000, 100);
}

void World::createWorld(sf::Event& event) {

    colMan.moveAndSeparateCells(cellGroup, map, window, 200);
    //std::cout << "After move: Cell A Position: " << cellGroup.getCellGroup()[0]->getPosition().x << ", " << cellGroup.getCellGroup()[0]->getPosition().y << std::endl;

    colMan.checkPelletCollisionForEveryCell(window, cellGroup, map, pelHan.getActivePellets());
    colMan.assignPellets(pelHan.getActivePellets(), map);
    pelHan.drawPellets(window, map, 10000);
    pelHan.eraseIfInactive();
    map.drawGrid(window);
    cellGroup.draw(window);

    if (gameOver) {
        debug.drawInformation(window, "Game Over! Press R to restart", 50, view.getCenter().x, view.getCenter().y);
        return;
    }

    displayInfo();

    colMan.assignCells(cellGroup, map);

    //std::cout << "After separation: Cell A Position: " << cellGroup.getCellGroup()[0]->getPosition().x << ", " << cellGroup.getCellGroup()[0]->getPosition().y << std::endl;

    view.setCenter(cellGroup.getAvgPos().x, cellGroup.getAvgPos().y);
    view.setSize(window.getDefaultView().getSize() * input.getZoomOffset());

    input.zoomWithScrollWheel(event);
    input.split(cellGroup);
    
    window.setView(view);
}

void World::displayInfo() {
    for (Circle* cell : cellGroup.getCellGroup()) {
        cell->setColor(sf::Color::Blue);
        debug.drawVector(cell->getPosition(), cell->getDirection(), window, 100.0f, sf::Color::Red);
        debug.drawInformation(window, "Mass: " + std::to_string(cell->getCircleSize()), 10, cell->getPosition().x, cell->getPosition().y);
    }
}

void World::restart() {
    gameOver = false;
    cellGroup.reset();
    addSize = 0;
}

bool World::isGameOver() const {
    return gameOver;
}