//
// Created by 김태현 on 2022/02/13.
//

#include "DIK.h"

DIK::DIK(int maxRow, int maxCol, Maze &maze) : maze(maze), maxRow(maxRow), maxCol(maxCol) {
    
    distTable = std::make_unique<std::unique_ptr<int[]>[]>(maxCol);
    for (int column = 0; column < maxCol; column++) {
        distTable[column] = std::make_unique<int[]>(maxRow);
        for (int row = 0; row < maxRow; row++) {
            distTable[column][row] = INF;
        }
    }
}

void DIK::findSP() {
    // Insert starting point to found set.
    foundLocationSet.push_back(start);

    distTable[start->col][start->row] = 0;

    Location *currentLoc = start;
    int closestIndex = 0;

    // Finding the shortest path.
    while (currentLoc->row != end->row || currentLoc->col != end->col){
        // 1. Update the distance to all vertices adjacent to the found location set.
        UpdateDist(currentLoc);

        // 2. Find the vertex which has minimum distance.
        int minDist = INF;
        for (int i = 0; i < adjacentSet.size(); ++i) {
            if (distTable[adjacentSet[i]->col][adjacentSet[i]->row] < minDist){
                minDist = distTable[adjacentSet[i]->col][adjacentSet[i]->row];
                currentLoc = adjacentSet[i];
                closestIndex = i;
            }
        }

        // 3. Insert that minimum vertex to the found location set.
        foundLocationSet.push_back(currentLoc);

        // 4. Delete that minimum vertex from the adjacent location set.
        adjacentSet.erase(adjacentSet.begin() + closestIndex);
    }
}

void DIK::UpdateDist(Location *currentLoc) {
    // There are ways to improve performance at this point.
    // Such as data structure of adjacent vertices set.
    for (int dir = 0; dir < 4; dir++) {
        // For the adjacent currentLoc from all found locations,
        // if the adjacent currentLoc is not in the found currentLoc set,
        // calculate minimum distance and update if it is needed.
        // The edge vertices of maze are have nullptr for limits of maze size.
        Location * adLoc = maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);
        if (adLoc != nullptr &&
            std::find(foundLocationSet.begin(), foundLocationSet.end(), adLoc) ==
            foundLocationSet.end()) {
            if (std::find(adjacentSet.begin(), adjacentSet.end(), adLoc) == adjacentSet.end()) {
                adjacentSet.push_back(adLoc);
            }
            if (distTable[adLoc->col][adLoc->row] >
                distTable[currentLoc->col][currentLoc->row] + currentLoc->weight[dir]) {
                distTable[adLoc->col][adLoc->row] =
                        distTable[currentLoc->col][currentLoc->row] + currentLoc->weight[dir];
            }
        }
    }
}

void DIK::printLocationDistSet() const {
    for (int col = 0; col < maxCol; col++) {
        for (int row = 0; row < maxRow; row++) {
            std::cout<<distTable[col][row] <<" ";
        }
        std::cout<<std::endl;
    }
}



