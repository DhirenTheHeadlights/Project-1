#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Map.h"
#include "Pellet.h"
#include "Circle.h"
#include <variant>

struct GridObject {
    std::variant<Pellet, Circle> object;
};

class Hashmap {
public:
    void assignPellet(const Pellet& pellet);
    void assignCircle(const Circle& circle);
    bool checkCollision(Circle& circle, Pellet& pellet, Hashmap hashmap);
    std::vector<Pellet> getPelletsInSameCell(float x, float y) const;
private:
    std::unordered_map<std::string, std::vector<GridObject>> hashmap;
    std::string generateKey(int x, int y) const;
};