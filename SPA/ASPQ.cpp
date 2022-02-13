//
// Created by 김태현 on 2022/02/13.
//

#include "ASPQ.h"

ASPQ::ASPQ(int maxRow, int maxCol, Maze &maze) : maze(maze), maxRow(maxRow), maxCol(maxCol) {
    distTable = new int*[maxCol];
    for (int column = 0; column < maxCol; column++) {
        distTable[column] = new int[maxRow];
        for (int row = 0; row < maxRow; row++) {
            distTable[column][row] = INF;
        }
    }
}

ASPQ::~ASPQ() {
    for (int i = 0; i < maxCol; i++){
        delete distTable[i];
    }
    delete distTable;
}

void ASPQ::printShortestPath() const {
    for (int col = 0; col < maxCol; col++) {
        for (int row = 0; row < maxRow; row++) {
            std::cout<<distTable[col][row]<<" ";
        }
        std::cout<<std::endl;
    }
}

void ASPQ::printLocationDistSet() const {
    for (int col = 0; col < maxCol; col++) {
        for (int row = 0; row < maxRow; row++) {
            if (distTable[col][row] != INF) {
                std::cout <<  "0 ";
            } else{
                std::cout <<  "- ";
            }
        }
        std::cout<<std::endl;
    }
}

void ASPQ::findSP() {

    distTable[start->col][start->row] = 0;

    // Initially push the starting point to PQ.
    adjacentLocQueue.push({-(distTable[start->col][start->row] +
    ((std::abs(end->row - start->row)) * (WEIGHT_MAX + WEIGHT_MIN) / 2) +
    ((std::abs(end->col - start->col)) * (WEIGHT_MAX + WEIGHT_MIN) / 2)),
                           start});

    Location *currentLoc = nullptr;

    while (currentLoc->row != end->row || currentLoc->col != end->col){

        // Dequeue the closest location.
        // The distance of location from the starting point is used for only sorting.
        currentLoc = adjacentLocQueue.top().second;
        adjacentLocQueue.pop();

        // Update distance table for adjacent locations.
        UpdateDist(currentLoc);
    }
}

void ASPQ::UpdateDist(Location *currentLoc) {
    for (int dir = 0; dir < 4; ++dir) {
        Location* adjacent = maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);
        if (adjacent != nullptr) {

            // If there is adjacent location exists,
            // and its new distance is shorter than distance in the table, update it.
            if (distTable[adjacent->col][adjacent->row] >
                currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]) {

                distTable[adjacent->col][adjacent->row] =
                        currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row];


                // Enqueue the new adjacent location which is updated just before.
                adjacentLocQueue.push(
                        {-(distTable[adjacent->col][adjacent->row] +
                           ((std::abs(end->row - adjacent->row)) * (WEIGHT_MAX + WEIGHT_MIN) / 2) +
                           ((std::abs(end->col - adjacent->col)) * (WEIGHT_MAX + WEIGHT_MIN) / 2)),
                         adjacent});
            }
        }
    }
}

