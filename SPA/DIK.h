//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_DIK_H
#define SPA_PICO_DIK_H


#include "../Maze.h"
#include "SPA.h"
#include <memory>
#include <algorithm>

class DIK : public SPA{
private:

    //The starting point and destination of route.
    Location* end;
    Location* start;

    Maze &maze;
    unsigned short maxRow, maxCol;

    // Distance table that stores shortest distance of all locations and their previous location.
    unsigned short** distTable;

    // Set of locations that are found.
    std::vector<Location*> foundLocationSet;

    // Vector of adjacent locations to found set.
    std::vector<Location*> adjacentSet;



    // Update distance of all existing adjacent.
    void UpdateDist(Location *currentLoc);

public:
    DIK(unsigned short maxRow, unsigned short maxCol, Maze &maze);
    ~DIK();

    void findSP() override;

    void setStart(unsigned short row, unsigned short column) override{
        start = &(maze.location[column][row]);
    }

    void setEnd(unsigned short row, unsigned short column) override{
        end = &(maze.location[column][row]);
    }

    int getShortestPathLength() const override {
        return distTable[end->col][end->row];
    }

    std::string getTypeName() const override{
        return std::string("DIK   ");
    }
    void printLocationDistSet() const;
};


#endif //SPA_PICO_DIK_H
