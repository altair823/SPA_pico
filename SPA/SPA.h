//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_SPA_H
#define SPA_PICO_SPA_H


#include "../Maze.h"

class SPA {

    // Set starting point.
    virtual void setStart(unsigned short row, unsigned short column) = 0;

    // Set ending point.
    virtual void setEnd(unsigned short row, unsigned short column) = 0;

    // Find the shortest path.
    virtual void findSP() = 0;

    // Get length of the shortest path.
    virtual int getShortestPathLength() const = 0;

    // Get the class name of SPA object which indicate the algorithm type.
    virtual std::string getTypeName() const = 0;
};


#endif //SPA_PICO_SPA_H
