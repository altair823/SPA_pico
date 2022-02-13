//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_ASBQ_H
#define SPA_PICO_ASBQ_H

#include "../structure/BucketQueue.h"
#include "SPA.h"

class ASBQ : public SPA {
private:
    int maxWeight = 0;
    Maze &maze;

    //The starting point and destination of route.
    Location *end;
    Location *start;

    int maxRow, maxCol;

    // Distance table that stores shortest distance of all locations.
    int **distTable;

    // Priority Queue of adjacent locations to found set.
    // The int values indicate distance between current location and adjacent location +
    // estimated distance between adjacent location and destination.
    BucketQueue<int, Location> *adjacentLocQueue;

    // Update distance of new adjacent location only.
    void UpdateDist(Location *currentLoc);

public:
    ASBQ(int maxRow, int maxCol, Maze &maze);

    ~ASBQ();

    void setStart(int row, int column) override {
        start = &(maze.location[column][row]);
    }

    void setEnd(int row, int column) override {
        end = &(maze.location[column][row]);
    }

    void findSP() override;

    int getShortestPathLength() const override {
        return distTable[end->col][end->row];
    }

    std::string getTypeName() const override {
        return "ASBQ  ";
    }

    void printLocationDistSet() const;
};



#endif //SPA_PICO_ASBQ_H
