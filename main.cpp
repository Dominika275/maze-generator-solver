#include <iostream>
#include "Maze.h"

int main() {
    Maze maze;

    int width = 35; 
    int height = 19;

    maze.generateRandom(width, height);
    std::vector<Point> path = maze.findShortestPath();

    if (path.empty()) {
        std::cout << "No path found from A to B.\n";
    }
    else {
        std::cout << "Maze generated successfully.\n";
        std::cout << "Shortest path from A (Top-Left) to B (Bottom-Right) marked with '.':\n\n";
        
        maze.printGridWithPath(path);
        
        std::cout << "\nSteps taken in shortest path: " << path.size() << '\n';
    }

    return 0;
}