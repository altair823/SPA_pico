/**
 * @file ASBQ.h
 * @date 2022/02/17
 * @author altair823
 * @version 1.0
 * @brief Implementation of A* algorithm using bucket queue.
 */

#ifndef SPA_PICO_ASBQ_H
#define SPA_PICO_ASBQ_H

#include "../structure/BucketQueue.h"
#include "../Maze.h"
#include "SPA.h"

/**
 * A* algorithm class that finds the shortest path of given maze.
 * The data types are essential for optimizing the space complexity.
 * @tparam T The data type of row and column of maze.
 * @tparam W The data type of the maze's weights.
 */
template <typename T, typename W>
class ASBQ : public SPA<T, W> {
private:

    /**
     * The maze reference variable.
     */
    Maze<T, W> &maze;

    /**
     * The starting point for the shortest path of maze.
     */
    Location<T, W> *end;

    /**
     * The destination point for the shortest path of maze.
     */
    Location<T, W> *start;

    /**
     * The Max size of the row and the column in maze.
     */
    T maxRow, maxCol;

    /**
     * The table containing the distances for all locations from the starting point.
     */
    W** distTable;

    /**
     * The priority Queue for storing given distance of adjacent location and to get the nearest location.
     * The key represents the sum of the distance from the current location to the adjacent location
     * and the estimated distance from the adjacent location to the destination.
     */
    BucketQueue<W, Location<T, W>*> adjacentLocQueue;

    /**
     * Updates distances of all adjacent locations from the current location.
     * @param currentLoc Current location.
     */
    void UpdateDist(Location<T, W> *currentLoc);

public:
    ASBQ(T maxRow, T maxCol, Maze<T, W> &maze);
    ~ASBQ();
    void setStart(T row, T column) override {start = &(maze.location[column][row]);}
    void setEnd(T row, T column) override {end = &(maze.location[column][row]);}
    void findSP() override;
    W getShortestPathLength() const override {return distTable[end->col][end->row];}
    std::string getTypeName() const override {return "ASBQ  ";}
    void printLocationDistSet() const;
};

template<typename T, typename W>
ASBQ<T, W>::ASBQ(T maxRow, T maxCol, Maze<T, W> &maze) :
            maze(maze),
            maxRow(maxRow),
            maxCol(maxCol){
        distTable = new W*[maxCol];
        for (T column = 0; column < maxCol; column++) {
            distTable[column] = new W[maxRow];
            for (T row = 0; row < maxRow; row++) {
                distTable[column][row] = INF;
            }
        }
    }

template<typename T, typename W>
ASBQ<T, W>::~ASBQ() {
    for (T i = 0; i < maxCol; i++) {
        delete distTable[i];
    }
    delete distTable;
}

template<typename T, typename W>
void ASBQ<T, W>::findSP() {
    distTable[start->col][start->row] = 0;
    // Initially push the starting point to PQ.
    adjacentLocQueue.push((W) (distTable[start->col][start->row] +
                                (ABS_MIN_WEIGHT(end, start)) +
                                (ABS_MIN_WEIGHT(end, start))),
                           start);
    auto currentLoc = start;
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
void ASBQ<T, W>::UpdateDist(Location<T, W> *currentLoc) {
    for (char dir = 0; dir < 4; ++dir) {
        auto adjacent = maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);
        // If there is adjacent location exists,
        // and its new distance is shorter then distance in the table, update it.
        if (adjacent != nullptr &&
            distTable[adjacent->col][adjacent->row] >
            currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]) {

            distTable[adjacent->col][adjacent->row] =
                    (W) (currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]);

            // Enqueue the new adjacent location which is updated just before.
            adjacentLocQueue.push(
                    (W) (distTable[adjacent->col][adjacent->row] +
                         (ABS_MIN_WEIGHT(end, adjacent)) +
                         (ABS_MIN_WEIGHT(end, adjacent))),
                    adjacent);

        }
    }
}

template<typename T, typename W>
void ASBQ<T, W>::printLocationDistSet() const {
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


#endif //SPA_PICO_ASBQ_H
