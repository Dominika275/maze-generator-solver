#include "Maze.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <random>

void Maze::generateRandom(int width, int height) {
    if (width % 2 == 0) width++;
    if (height % 2 == 0) height++;

    grid.assign(height, std::string(width, '#'));

    std::vector<std::vector<bool>> visitedCells(height, std::vector<bool>(width, false));
    std::vector<Point> activeCells;

    Point current = { 1, 1 };
    visitedCells[current.y][current.x] = true;
    grid[current.y][current.x] = ' ';
    activeCells.push_back(current);

    std::random_device rd;
    std::mt19937 gen(rd());

    while (!activeCells.empty()) {
        std::uniform_int_distribution<> dist(0, activeCells.size() - 1);
        int randomIndex = dist(gen);
        current = activeCells[randomIndex];

        std::vector<std::pair<Point, Point>> neighbors;
        std::vector<Point> dirs = { {0, 2}, {2, 0}, {0, -2}, {-2, 0} };

        for (const auto& d : dirs) {
            Point nextCell = { current.x + d.x, current.y + d.y };
            Point wall = { current.x + d.x / 2, current.y + d.y / 2 };

            if (nextCell.y > 0 && nextCell.y < height - 1 &&
                nextCell.x > 0 && nextCell.x < width - 1) {
                if (!visitedCells[nextCell.y][nextCell.x]) {
                    neighbors.push_back({ nextCell, wall });
                }
            }
        }

        if (!neighbors.empty()) {
            std::shuffle(neighbors.begin(), neighbors.end(), gen);
            auto chosen = neighbors[0];

            grid[chosen.second.y][chosen.second.x] = ' ';
            grid[chosen.first.y][chosen.first.x] = ' ';

            visitedCells[chosen.first.y][chosen.first.x] = true;
            activeCells.push_back(chosen.first);
        } else {
            activeCells.erase(activeCells.begin() + randomIndex);
        }
    }

    start = {1, 1};
    grid[start.y][start.x] = 'A';

    end = {width - 2, height - 2};
    grid[end.y][end.x] = 'B';
}

std::vector<Point> Maze::findShortestPath() {
    std::vector<Point> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
    std::unordered_map<Point, Point, PointHash> predecessors;
    std::queue<Point> q;
    q.push(start);

    std::unordered_map<Point, bool, PointHash> visited;
    visited[start] = true;

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        if (current == end) break;

        for (const auto& dir : directions) {
            Point neighbor = { current.x + dir.x, current.y + dir.y };

            if (isValid(neighbor) && !visited[neighbor]) {
                visited[neighbor] = true;
                predecessors[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    std::vector<Point> path;
    if (predecessors.find(end) == predecessors.end()) return path;

    for (Point at = end; at != start; at = predecessors[at]) {
        path.push_back(at);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

void Maze::printGridWithPath(const std::vector<Point>& path) const {
    std::vector<std::string> gridCopy = grid;
    for (const auto& p : path) {
        if (gridCopy[p.y][p.x] == ' ') {
            gridCopy[p.y][p.x] = '.';
        }
    }

    for (const auto& line : gridCopy) {
        std::cout << line << '\n';
    }
}

bool Maze::isValid(const Point& p) const {
    return p.y >= 0 && p.y < grid.size() &&
        p.x >= 0 && p.x < grid[0].size() &&
        (grid[p.y][p.x] == ' ' || grid[p.y][p.x] == 'B');
}