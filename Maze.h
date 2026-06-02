#pragma once
#include <vector>
#include <string>
#include "Point.h"

class Maze {
public:
    std::vector<std::string> grid;
    Point start, end;

    void generateRandom(int width, int height);
    std::vector<Point> findShortestPath();
    void printGridWithPath(const std::vector<Point>& path) const;

private:
    bool isValid(const Point& p) const;
};