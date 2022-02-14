//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_ASBQ_H
#define SPA_PICO_ASBQ_H


#include "../structure/BucketQueue.h"
#include "SPA.h"


class ASBQ : public SPA {
private:
    short maxWeight = 0;
    Maze &maze;

    //The starting point and destination of route.
    Location *end;
    Location *start;

    unsigned short maxRow, maxCol;

    // Distance table that stores shortest distance of all locations.
    short **distTable;

    // Priority Queue of adjacent locations to found set.
    // The int values indicate distance between current location and adjacent location +
    // estimated distance between adjacent location and destination.
    BucketQueue<short, Location> *adjacentLocQueue;

    // Update distance of new adjacent location only.
    void UpdateDist(Location *currentLoc);

public:
    ASBQ(unsigned short maxRow, unsigned short maxCol, Maze &maze);

    ~ASBQ();

    void setStart(unsigned short row, unsigned short column) override {
        start = &(maze.location[column][row]);
    }

    void setEnd(unsigned short row, unsigned short column) override {
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
