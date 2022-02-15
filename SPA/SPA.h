//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_SPA_H
#define SPA_PICO_SPA_H


#include "../Maze.h"

template <typename T, typename W>
class SPA {

    // Set starting point.
    virtual void setStart(T row, T column) = 0;

    // Set ending point.
    virtual void setEnd(T row, T column) = 0;

    // Find the shortest path.
    virtual void findSP() = 0;

    // Get length of the shortest path.
    virtual W getShortestPathLength() const = 0;

    // Get the class name of SPA object which indicate the algorithm type.
    virtual std::string getTypeName() const = 0;
};


#endif //SPA_PICO_SPA_H
