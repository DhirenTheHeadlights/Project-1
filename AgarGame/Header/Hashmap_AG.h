#pragma once
#include "Pellet_AG.h"
#include "Circle_AG.h" 
#include "Globals.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <set>

class Hashmap {
public:
    Hashmap(sf::RenderWindow& window);
    void assignPellet(Pellet& pellet, Map& map);
    void assignCell(Circle& cell, Map& map);
    std::set<Circle*> getCellsInSameCell(Circle* circle, Map& map) const;
    std::string generateKey(int x, int y) const;
    const std::unordered_map<std::string, std::set<Pellet*>>& getPelletHashmap() const;
    const std::unordered_map<std::string, std::set<Circle*>>& getCellHashmap() const;
private:
    std::unordered_map<std::string, std::set<Pellet*>> pelletHashmap;
    std::unordered_map<std::string, std::set<Circle*>> cellHashmap;
    std::set<Pellet*> getPelletsInSameCell(float x, float y) const;
    float minDistForCollision = 0.5f;
};