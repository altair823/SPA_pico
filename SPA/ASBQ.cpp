//
// Created by 김태현 on 2022/02/13.
//

#include "ASBQ.h"

ASBQ::ASBQ(int maxRow, int maxCol, Maze &maze)
            : maze(maze),
            maxRow(maxRow),
            maxCol(maxCol),
            adjacentLocQueue(new BucketQueue<int, Location>(maxRow, maxCol, (WEIGHT_MAX + WEIGHT_MIN) / 2)) {
    distTable = new int*[maxCol];
    for (int i = 0; i < maxCol; i++){
        distTable[i] = new int[maxRow];
    }
}

ASBQ::~ASBQ() {
    delete adjacentLocQueue;

    for (int i = 0; i < maxCol; i++){
        delete distTable[i];
    }
    delete distTable;
}
void ASBQ::UpdateDist(Location *currentLoc) {
    for (int dir = 0; dir < 4; ++dir) {
        Location* adjacent = maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);
        // If there is adjacent location exists,
        // and its new distance is shorter then distance in the table, update it.
        if (adjacent != nullptr &&
            distTable[adjacent->col][adjacent->row] >
            currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]) {

            distTable[adjacent->col][adjacent->row] =
                    currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row];


            // Enqueue the new adjacent location which is updated just before.
            adjacentLocQueue->Insert(
                    distTable[adjacent->col][adjacent->row] +
                    ((std::abs(end->row - adjacent->row)) * (WEIGHT_MAX + WEIGHT_MIN) / 2) +
                    ((std::abs(end->col - adjacent->col)) * (WEIGHT_MAX + WEIGHT_MIN) / 2),
                    adjacent);

        std::cout<<"11"<<std::endl;
            if (maxWeight < distTable[adjacent->col][adjacent->row] +
            ((std::abs(end->row - adjacent->row)) * (WEIGHT_MAX + WEIGHT_MIN) / 2) +
            ((std::abs(end->col - adjacent->col)) * (WEIGHT_MAX + WEIGHT_MIN) / 2)){
                maxWeight = distTable[adjacent->col][adjacent->row] +
                        ((std::abs(end->row - adjacent->row)) * (WEIGHT_MAX + WEIGHT_MIN) / 2) +
                        ((std::abs(end->col - adjacent->col)) * (WEIGHT_MAX + WEIGHT_MIN) / 2);
            }
        }
    }
}


void ASBQ::findSP() {
    distTable[start->col][start->row] = 0;

    // Initially push the starting point to PQ.
    adjacentLocQueue->Insert(distTable[start->col][start->row] +
    ((std::abs(end->row - start->row)) * (WEIGHT_MAX + WEIGHT_MIN) / 2) +
    ((std::abs(end->col - start->col)) * (WEIGHT_MAX + WEIGHT_MIN) / 2),
    start);

    Location* currentLoc = start;

    while (currentLoc->row != end->row || currentLoc->col != end->col){

        // Dequeue the closest location.
        // The distance of location from the starting point is used for only sorting.
        int currentDist = adjacentLocQueue->getMinimumKey();
        currentLoc = adjacentLocQueue->PopMinimum();
        std::cout << "curLoc: " << currentLoc->row << ", " << currentLoc->col << std::endl;

        // Update distance table for adjacent locations.
        UpdateDist(currentLoc);
    }
    std::cout << "distLoc: " << currentLoc->row << ", " << currentLoc->col << std::endl;
}

void ASBQ::printLocationDistSet() const {
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


