//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_ASPQ_H
#define SPA_PICO_ASPQ_H

#include <queue>
#include <cmath>
#include <cstdlib>
#include <memory>
#include "../Maze.h"
#include "SPA.h"

class ASPQ : public SPA{
private:

    //The starting point and destination of route.
    Location *end;
    Location *start;

    Maze &maze;
    int maxRow, maxCol;

    // Distance table that stores the shortest distance of all locations and their previous location.
    int** distTable;

    // Priority Queue of adjacent locations to found set.
    // The int values indicate distance between current location and adjacent location +
    // estimated distance between adjacent location and destination.
    std::priority_queue<std::pair<int, Location *>> adjacentLocQueue;

    // Update distance of new adjacent location only.
    void UpdateDist(Location *currentLoc);

public:
    ASPQ(int maxRow, int maxCol, Maze &maze);
    ~ASPQ();

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
        return "ASPQ  ";
    }

    void printShortestPath() const;

    void printLocationDistSet() const;
};
#endif //SPA_PICO_ASPQ_H
