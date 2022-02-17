//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_ASPQ_H
#define SPA_PICO_ASPQ_H

#include "../structure/PriorityQueue.h"
#include "../Maze.h"
#include "SPA.h"

template <typename T, typename W>
class ASPQ : public SPA<T, W> {
private:

    //The starting point and destination of route.
    Location<T, W> *end{};
    Location<T, W> *start{};

    Maze<T, W> &maze;
    T maxRow, maxCol;

    // Distance table that stores the shortest distance of all locations and their previous location.
    W** distTable;

    // Priority Queue of adjacent locations to found set.
    // The int values indicate distance between current location and adjacent location +
    // estimated distance between adjacent location and destination.
    PriorityQueue<W, Location<T, W>*> adjacentLocQueue;

    // Update distance of new adjacent location only.
    void UpdateDist(Location<T, W> *currentLoc);

public:
    ASPQ(T maxRow, T maxCol, Maze<T, W> &maze);
    ~ASPQ();
    void setStart(T row, T column) override {start = &(maze.location[column][row]);}
    void setEnd(T row, T column) override {end = &(maze.location[column][row]);}
    void findSP() override;
    W getShortestPathLength() const override {return distTable[end->col][end->row];}
    std::string getTypeName() const override {return "ASPQ  ";}
    void printShortestPath() const;
    void printLocationDistSet() const;
};

template<typename T, typename W>
void ASPQ<T, W>::UpdateDist(Location<T, W> *currentLoc) {
    for (char dir = 0; dir < 4; ++dir) {
        auto adjacent = maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);
        if (adjacent != nullptr &&
            distTable[adjacent->col][adjacent->row] >
            currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]) {

            distTable[adjacent->col][adjacent->row] =
                    (W) (currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]);

            // Enqueue the new adjacent location which is updated just before.
            adjacentLocQueue.push(
                    -(distTable[adjacent->col][adjacent->row] +
                      (ABS_MIN_WEIGHT(end, adjacent)) +
                      (ABS_MIN_WEIGHT(end, adjacent))),
                    adjacent);
        }
    }
}

template<typename T, typename W>
ASPQ<T, W>::ASPQ(T maxRow, T maxCol, Maze<T, W> &maze): maze(maze), maxRow(maxRow), maxCol(maxCol) {
    distTable = new W *[maxCol];
    for (T column = 0; column < maxCol; column++) {
        distTable[column] = new W[maxRow];
        for (T row = 0; row < maxRow; row++) {
            distTable[column][row] = INF;
        }
    }
}

template<typename T, typename W>
ASPQ<T, W>::~ASPQ() {
    for (T i = 0; i < maxCol; i++) {
        delete distTable[i];
    }
    delete distTable;
}

template<typename T, typename W>
void ASPQ<T, W>::findSP() {
    distTable[start->col][start->row] = 0;
    // Initially push the starting point to PQ.
    adjacentLocQueue.push(-(distTable[start->col][start->row] +
                            (ABS_MIN_WEIGHT(end, start)) +
                            (ABS_MIN_WEIGHT(end, start))),
                          start);
    Location<T, W> *currentLoc = nullptr;
    while (currentLoc->row != end->row || currentLoc->col != end->col) {
        // Dequeue the closest location.
        // The distance of location from the starting point is used for only sorting.
        currentLoc = adjacentLocQueue.top();
        adjacentLocQueue.pop();
        // Update distance table for adjacent locations.
        UpdateDist(currentLoc);
    }
}

template<typename T, typename W>
void ASPQ<T, W>::printShortestPath() const {
    for (T col = 0; col < maxCol; col++) {
        for (T row = 0; row < maxRow; row++) {
            std::cout << distTable[col][row] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T, typename W>
void ASPQ<T, W>::printLocationDistSet() const {
    for (T col = 0; col < maxCol; col++) {
        for (T row = 0; row < maxRow; row++) {
            if (distTable[col][row] != INF) {
                std::cout << "0 ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
}

#endif //SPA_PICO_ASPQ_H
