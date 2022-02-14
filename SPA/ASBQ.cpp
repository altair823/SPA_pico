//
// Created by 김태현 on 2022/02/13.
//

#include "ASBQ.h"

ASBQ::ASBQ(unsigned short maxRow, unsigned short maxCol, Maze &maze)
            : maze(maze),
            maxRow(maxRow),
            maxCol(maxCol),
            adjacentLocQueue(new BucketQueue<short, Location>(maxRow, maxCol, (WEIGHT_MAX + WEIGHT_MIN) / 2)) {
    distTable = new short*[maxCol];
    for (int column = 0; column < maxCol; column++){
        distTable[column] = new short[maxRow];
        for (int row = 0; row < maxRow; row++){
            distTable[column][row] = INF;
        }
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
                    (short )(currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]);

            // Enqueue the new adjacent location which is updated just before.
            adjacentLocQueue->Insert(
                    (short )(distTable[adjacent->col][adjacent->row] +
                    (ABS_MIN_WEIGHT(end, adjacent)) +
                    (ABS_MIN_WEIGHT(end, adjacent))),
                    *adjacent);

            if (maxWeight < distTable[adjacent->col][adjacent->row] +
            (ABS_MIN_WEIGHT(end, adjacent)) +
            (ABS_MIN_WEIGHT(end, adjacent))){
                maxWeight = (short )(distTable[adjacent->col][adjacent->row] +
                        (ABS_MIN_WEIGHT(end, adjacent)) +
                        (ABS_MIN_WEIGHT(end, adjacent)));
            }
        }
    }
}


void ASBQ::findSP() {
    distTable[start->col][start->row] = 0;

    // Initially push the starting point to PQ.
    adjacentLocQueue->Insert((short )(distTable[start->col][start->row] +
    (ABS_MIN_WEIGHT(end, start)) +
    (ABS_MIN_WEIGHT(end, start))),
    *start);

    Location* currentLoc = start;

    while (currentLoc->row != end->row || currentLoc->col != end->col){

        // Dequeue the closest location.
        // The distance of location from the starting point is used for only sorting.
        currentLoc = adjacentLocQueue->PopMinimum();

        // Update distance table for adjacent locations.
        UpdateDist(currentLoc);
    }
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


