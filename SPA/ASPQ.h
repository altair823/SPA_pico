/**
 * @file ASPQ.h
 * @date 2022/02/17
 * @author altair823
 * @version 1.0
 * @brief Implementation of A* algorithm using a priority queue.
 */

#ifndef SPA_PICO_ASPQ_H
#define SPA_PICO_ASPQ_H

#include "../structure/PriorityQueue.h"
#include "../Maze.h"
#include "SPA.h"

/**
 * A* algorithm implementation class that finds the shortest path of given maze using a priority queue.
 * The data types are essential for optimizing the space complexity.
 * @tparam T The data type of row and column of maze.
 * @tparam W The data type of the maze's weights.
 */
template <typename T, typename W>
class ASPQ : public SPA<T, W> {
private:

    /**
     * The Max size of the row and the column in maze.
     */
    T maxRow, maxCol;

    /**
     * The table containing the distances for all locations from the starting point.
     */
    W** distTable;

    /**
     * The priority queue for storing given distance of adjacent location and to get the nearest location.
     * The key represents the sum of the distance from the current location to the adjacent location
     * and the estimated distance from the adjacent location to the destination.
     */
    PriorityQueue<W, Location<T, W>*> adjacentLocQueue;

    /**
     * Updates distances of all adjacent locations from the current location.
     * @param currentLoc Current location.
     */
    void UpdateDist(Location<T, W> *currentLoc);

public:
    /**
     * Constructor for A* algorithm class with priority queue.
     * @param maxRow Max row size of the given maze.
     * @param maxCol Max column size of the given maze.
     * @param maze The reference variable for the maze.
     */
    ASPQ(T maxRow, T maxCol, Maze<T, W> &maze);

    /**
     * Destructor for A* algorithm class with priority queue.
     */
    ~ASPQ();

    void findSP() override;
    W getShortestPathLength() const override {return distTable[this->end->col][this->end->row];}
    [[nodiscard]] std::string getTypeName() const override {return "ASPQ";}
};

template<typename T, typename W>
void ASPQ<T, W>::UpdateDist(Location<T, W> *currentLoc) {
    for (char dir = 0; dir < 4; ++dir) {
        auto adjacent = this->maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);
        if (adjacent != nullptr &&
            distTable[adjacent->col][adjacent->row] >
            currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]) {

            distTable[adjacent->col][adjacent->row] =
                    (W) (currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]);

            // Enqueue the new adjacent location which is updated just before.
            adjacentLocQueue.push(
                    -(distTable[adjacent->col][adjacent->row] +
                      (ABS_MIN_WEIGHT(this->end, adjacent)) +
                      (ABS_MIN_WEIGHT(this->end, adjacent))),
                    adjacent);
        }
    }
}

template<typename T, typename W>
ASPQ<T, W>::ASPQ(T _maxRow, T _maxCol, Maze<T, W> &_maze):
maxRow(_maxRow),
maxCol(_maxCol),
SPA<T, W>(_maze) {
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
    distTable[this->start->col][this->start->row] = 0;
    // Initially push the starting point to PQ.
    adjacentLocQueue.push(-(distTable[this->start->col][this->start->row] +
                            (ABS_MIN_WEIGHT(this->end, this->start)) +
                            (ABS_MIN_WEIGHT(this->end, this->start))),
                          this->start);
    Location<T, W> *currentLoc = nullptr;
    while (currentLoc->row != this->end->row || currentLoc->col != this->end->col) {
        // Dequeue the closest location.
        // The distance of location from the starting point is used for only sorting.
        currentLoc = adjacentLocQueue.top();
        adjacentLocQueue.pop();
        // Update distance table for adjacent locations.
        UpdateDist(currentLoc);
    }
}

#endif //SPA_PICO_ASPQ_H
