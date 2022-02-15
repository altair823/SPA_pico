//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_ASBQ_H
#define SPA_PICO_ASBQ_H


#include "../structure/BucketQueue.h"
#include "SPA.h"

template <typename T, typename W>
class ASBQ : public SPA<T, W> {
private:
    W maxWeight = 0;
    Maze<T, W> &maze;

    //The starting point and destination of route.
    Location<T, W> *end;
    Location<T, W> *start;

    T maxRow, maxCol;

    // Distance table that stores shortest distance of all locations.
    W** distTable;

    // Priority Queue of adjacent locations to found set.
    // The int values indicate distance between current location and adjacent location +
    // estimated distance between adjacent location and destination.
    BucketQueue<W, Location<T, W>> *adjacentLocQueue;

    // Update distance of new adjacent location only.
    void UpdateDist(Location<T, W> *currentLoc) {
        for (char dir = 0; dir < 4; ++dir) {
            Location<T, W> *adjacent = maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);

            // If there is adjacent location exists,
            // and its new distance is shorter then distance in the table, update it.
            if (adjacent != nullptr &&
                distTable[adjacent->col][adjacent->row] >
                currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]) {

                distTable[adjacent->col][adjacent->row] =
                        (W) (currentLoc->weight[dir] + distTable[currentLoc->col][currentLoc->row]);

                // Enqueue the new adjacent location which is updated just before.
                adjacentLocQueue->Insert(
                        (W) (distTable[adjacent->col][adjacent->row] +
                                 (ABS_MIN_WEIGHT(end, adjacent)) +
                                 (ABS_MIN_WEIGHT(end, adjacent))),
                        *adjacent);

                if (maxWeight < distTable[adjacent->col][adjacent->row] +
                                (ABS_MIN_WEIGHT(end, adjacent)) +
                                (ABS_MIN_WEIGHT(end, adjacent))) {
                    maxWeight = (W) (distTable[adjacent->col][adjacent->row] +
                                         (ABS_MIN_WEIGHT(end, adjacent)) +
                                         (ABS_MIN_WEIGHT(end, adjacent)));
                }
            }
        }
    }

public:
    ASBQ(T maxRow, T maxCol, Maze<T, W> &maze) :
            maze(maze),
            maxRow(maxRow),
            maxCol(maxCol),
            adjacentLocQueue(new BucketQueue<W, Location<T, W>>(maxRow, maxCol, (WEIGHT_MAX + WEIGHT_MIN) / 2)) {
        distTable = new W*[maxCol];
        for (T column = 0; column < maxCol; column++) {
            distTable[column] = new W[maxRow];
            for (T row = 0; row < maxRow; row++) {
                distTable[column][row] = INF;
            }
        }
    }

    ~ASBQ() {
        delete adjacentLocQueue;

        for (T i = 0; i < maxCol; i++) {
            delete distTable[i];
        }
        delete distTable;
    }

    void setStart(T row, T column) override {
        start = &(maze.location[column][row]);
    }

    void setEnd(T row, T column) override {
        end = &(maze.location[column][row]);
    }

    void findSP() override {
        distTable[start->col][start->row] = 0;

        // Initially push the starting point to PQ.
        adjacentLocQueue->Insert((W) (distTable[start->col][start->row] +
                                          (ABS_MIN_WEIGHT(end, start)) +
                                          (ABS_MIN_WEIGHT(end, start))),
                                 *start);

        Location<T, W> *currentLoc = start;

        while (currentLoc->row != end->row || currentLoc->col != end->col) {

            // Dequeue the closest location.
            // The distance of location from the starting point is used for only sorting.
            currentLoc = adjacentLocQueue->PopMinimum();

            // Update distance table for adjacent locations.
            UpdateDist(currentLoc);
        }
    }

    W getShortestPathLength() const override {
        return distTable[end->col][end->row];
    }

    std::string getTypeName() const override {
        return "ASBQ  ";
    }

    void printLocationDistSet() const {
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
};



#endif //SPA_PICO_ASBQ_H
