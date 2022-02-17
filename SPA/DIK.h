//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_DIK_H
#define SPA_PICO_DIK_H


#include "../Maze.h"
#include "SPA.h"
#include <algorithm>

template <typename T, typename W>
class DIK : public SPA<T, W> {
private:

    //The starting point and destination of route.
    Location<T, W> *end{};
    Location<T, W> *start{};

    Maze<T, W> &maze;
    T maxRow, maxCol;

    // Distance table that stores shortest distance of all locations and their previous location.
    W** distTable;

    // Set of locations that are found.
    bool** foundLocationSet;

    // Vector of adjacent locations to found set.
    std::vector<Location<T, W>*> adjacentSet;


    // Update distance of all existing adjacent.
    void UpdateDist(Location<T, W> *currentLoc) {
        // There are ways to improve performance at this point.
        // Such as data structure of adjacent vertices set.
        for (char dir = 0; dir < 4; dir++) {
            // For the adjacent currentLoc from all found locations,
            // if the adjacent currentLoc is not in the found currentLoc set,
            // calculate minimum distance and update if it is needed.
            // The edge vertices of maze are have nullptr for limits of maze size.
            auto adLoc = maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);
            if (adLoc != nullptr && foundLocationSet[adLoc->col][adLoc->row] == false) {
                if (std::find(adjacentSet.begin(), adjacentSet.end(), adLoc) == adjacentSet.end()) {
                    adjacentSet.push_back(adLoc);
                }
                if (distTable[adLoc->col][adLoc->row] >
                    distTable[currentLoc->col][currentLoc->row] + currentLoc->weight[dir]) {
                    distTable[adLoc->col][adLoc->row] =
                            distTable[currentLoc->col][currentLoc->row] + currentLoc->weight[dir];
                }
            }
        }
    };

public:
    DIK(T maxRow, T maxCol, Maze<T, W> &maze) : maze(maze), maxRow(maxRow), maxCol(maxCol) {
        distTable = new W*[maxCol];
        foundLocationSet = new bool*[maxCol];
        for (T column = 0; column < maxCol; column++) {
            distTable[column] = new W[maxRow];
            foundLocationSet[column] = new bool[maxRow];
            for (T row = 0; row < maxRow; row++) {
                distTable[column][row] = INF;
                foundLocationSet[column][row] = false;
            }
        }
    }

    ~DIK() {
        for (T i = 0; i < maxCol; i++) {
            delete[] distTable[i];
            delete[] foundLocationSet[i];
        }
        delete[] distTable;
        delete[] foundLocationSet;
    }

    void findSP() override {
        // Insert starting point to found set.
        foundLocationSet[start->col][start->row] = true;

        distTable[start->col][start->row] = 0;

        auto currentLoc = start;
        int closestIndex = 0;

        // Finding the shortest path.
        while (currentLoc->row != end->row || currentLoc->col != end->col) {
            // 1. Update the distance to all vertices adjacent to the found location set.
            UpdateDist(currentLoc);

            // 2. Find the vertex which has minimum distance.
            int minDist = INF;
            for (T i = 0; i < adjacentSet.size(); ++i) {
                if (distTable[adjacentSet[i]->col][adjacentSet[i]->row] < minDist) {
                    minDist = distTable[adjacentSet[i]->col][adjacentSet[i]->row];
                    currentLoc = adjacentSet[i];
                    closestIndex = i;
                }
            }

            // 3. Insert that minimum vertex to the found location set.
            foundLocationSet[currentLoc->col][currentLoc->row] = true;

            // 4. Delete that minimum vertex from the adjacent location set.
            adjacentSet.erase(adjacentSet.begin() + closestIndex);
        }
    }

    void setStart(T row, T column) override {
        start = &(maze.location[column][row]);
    }

    void setEnd(T row, T column) override {
        end = &(maze.location[column][row]);
    }

    W getShortestPathLength() const override {
        return distTable[end->col][end->row];
    }

    std::string getTypeName() const override {
        return "DIK   ";
    }

    void printLocationDistSet() const {
        for (T col = 0; col < maxCol; col++) {
            for (T row = 0; row < maxRow; row++) {
                std::cout << distTable[col][row] << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif //SPA_PICO_DIK_H
