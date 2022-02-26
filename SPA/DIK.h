/**
 * @file DIK.h
 * @date 2022/02/17
 * @author altair823
 * @version 1.0
 * @brief The naive implementation of the dijkstra algorithm.
 */

#ifndef SPA_PICO_DIK_H
#define SPA_PICO_DIK_H

#include "../Maze.h"
#include "SPA.h"

/**
 * @def DEFAULT_LIST_CAP
 * Initial default capacity of the list that contains all adjacent locations.
 */
#define DEFAULT_LIST_CAP 10

/**
 * Naive implementation class of the dijkstra algorithm.
 * The data types are essential for optimizing the space complexity.
 * @tparam T The data type of row and column of maze.
 * @tparam W The data type of the maze's weights.
 */
template <typename T, typename W>
class DIK : public SPA<T, W> {
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
     * The boolean table indicating whether a particular location has been visited before.
     */
    bool** foundLocationSet;

    /**
     * The list that contains all adjacent locations in current state.
     */
    Location<T, W>** adjacentList;
    int adjacentListCap;
    int adjacentListTop;


    /**
     * Updates distances of all adjacent locations from the current location.
     * @param currentLoc Current location.
     */
    void UpdateDist(Location<T, W> *currentLoc);

public:
    /**
     * Constructor for Dijkstra algorithm class.
     * @param maxRow  Max row size of the given maze.
     * @param maxCol  Max column size of the given maze.
     * @param maze The reference variable for the maze.
     */
    DIK(T maxRow, T maxCol, Maze<T, W> &maze);

    /**
     * Destructor for dijkstra algorithm class.
     */
    ~DIK();

    void findSP() override;
    W getShortestPathLength() const override {return distTable[this->end->col][this->end->row];}
    [[nodiscard]] std::string getTypeName() const override {return "DIK ";}
};

template<typename T, typename W>
void DIK<T, W>::UpdateDist(Location<T, W> *currentLoc) {
    // There are ways to improve performance at this point.
    // Such as data structure of adjacent vertices set.
    for (char dir = 0; dir < 4; dir++) {
        // For the adjacent currentLoc from all found locations,
        // if the adjacent currentLoc is not in the found currentLoc set,
        // calculate minimum distance and update if it is needed.
        // The edge vertices of maze are have nullptr for limits of maze size.
        auto adLoc = this->maze.getAdjacentLoc(currentLoc->row, currentLoc->col, dir);
        if (adLoc != nullptr && foundLocationSet[adLoc->col][adLoc->row] == false) {
            bool found = false;
            for (int p = 0; p < adjacentListTop; p++){
                if (adjacentList[p] == adLoc){
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (adjacentListTop == adjacentListCap){
                    adjacentListCap *= 2;
                    auto t_adList = new Location<T, W>*[adjacentListCap];
                    for (int i = 0; i < adjacentListTop; i++){
                        t_adList[i] = adjacentList[i];
                    }
                    delete[] adjacentList;
                    adjacentList = t_adList;
                }
                adjacentList[adjacentListTop++] = adLoc;
            }
            if (distTable[adLoc->col][adLoc->row] >
                distTable[currentLoc->col][currentLoc->row] + currentLoc->weight[dir]) {
                distTable[adLoc->col][adLoc->row] =
                        distTable[currentLoc->col][currentLoc->row] + currentLoc->weight[dir];
            }
        }
    }
}

template<typename T, typename W>
DIK<T, W>::DIK(T _maxRow, T _maxCol, Maze<T, W> &_maze) :
maxRow(_maxRow),
maxCol(_maxCol),
adjacentList(new Location<T, W>*[DEFAULT_LIST_CAP]),
adjacentListCap(DEFAULT_LIST_CAP),
adjacentListTop(0),
SPA<T, W>(_maze){
    distTable = new W *[maxCol];
    foundLocationSet = new bool *[maxCol];
    for (T column = 0; column < maxCol; column++) {
        distTable[column] = new W[maxRow];
        foundLocationSet[column] = new bool[maxRow];
        for (T row = 0; row < maxRow; row++) {
            distTable[column][row] = INF;
            foundLocationSet[column][row] = false;
        }
    }
}

template<typename T, typename W>
DIK<T, W>::~DIK() {
    for (T i = 0; i < maxCol; i++) {
        delete[] distTable[i];
        delete[] foundLocationSet[i];
    }
    delete[] distTable;
    delete[] foundLocationSet;
    delete[] adjacentList;
}

template<typename T, typename W>
void DIK<T, W>::findSP() {
    // Insert starting point to found set.
    foundLocationSet[this->start->col][this->start->row] = true;

    distTable[this->start->col][this->start->row] = 0;

    auto currentLoc = this->start;
    int closestIndex = 0;

    // Finding the shortest path.
    while (currentLoc->row != this->end->row || currentLoc->col != this->end->col) {
        // 1. Update the distance to all vertices adjacent to the found location set.
        UpdateDist(currentLoc);

        // 2. Find the vertex which has minimum distance.
        int minDist = INF;
        for (T i = 0; i < adjacentListTop; ++i) {
            if (distTable[adjacentList[i]->col][adjacentList[i]->row] < minDist) {
                minDist = distTable[adjacentList[i]->col][adjacentList[i]->row];
                currentLoc = adjacentList[i];
                closestIndex = i;
            }
        }

        // 3. Insert that minimum vertex to the found location set.
        foundLocationSet[currentLoc->col][currentLoc->row] = true;

        // 4. Delete that minimum vertex from the adjacent location set.
        adjacentListTop--;
        for (int i = closestIndex; i < adjacentListTop; i++){
            adjacentList[i] = adjacentList[i+1];
        }
    }
}

#endif //SPA_PICO_DIK_H
