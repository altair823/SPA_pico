//
// Created by 김태현 on 2022/02/12.
//

#ifndef SPA_PICO_MAZE_H
#define SPA_PICO_MAZE_H

#include <iostream>
#include <random>
#include <set>
#include "pico/stdlib.h"
#include "hardware/structs/rosc.h"

#define UP 3
#define DOWN 2
#define LEFT 1
#define RIGHT 0

#define INF 10000  // Maximum weight of edges.
#define WEIGHT_MAX 5
#define WEIGHT_MIN 1

#define ABS_MIN_WEIGHT(x, y) ((x->row - y->row) > 0 ? ((x->row - y->row) * WEIGHT_MIN) : -((x->row - y->row) * WEIGHT_MIN))

struct Location{
    unsigned short row;
    unsigned short col;
    short weight[4];  // Edges to 4 adjacent cells.
};

struct Maze {
public:
    unsigned short maxRow;
    unsigned short maxColumn;
    Location **location;
    unsigned short* locationSet;
    unsigned short* nextLocationSet;
    unsigned short previouslyAssignedSetNumber;
    std::set<unsigned short> existingSet;

    /**
     * Initialize maze.
     * @param maxRow Max row size.
     * @param maxCol Max column size.
     */
    Maze(unsigned short maxRow, unsigned short maxCol);

    /**
     * Destructor of maze.
     */
    ~Maze();

    void makeMaze();

    void printMaze() const;
    Location* getAdjacentLoc(unsigned short row, unsigned short col, char dir) const;

private:

    /**
     * Open a single wall for specific direction in given location.
     * @param row Row of location.
     * @param column Column of location.
     * @param direction Direction of target wall.
     * @param weight A new weight of opened way.
     */
    void openWall(unsigned short row, unsigned short column, char direction, short weight);

    /**
     * Merge two adjacent cells randomly through open the wall and add a weight.
     * @param column Column number of target column.
     */
    void mergeRandomly(unsigned short column);

    static bool choiceRandomly();

    void mergeWithRight(unsigned short row, unsigned short column);

    void updateSet(unsigned short targetSetRow, unsigned short destSetRow);

    short generateWeight();

    void expandSetsVertical(unsigned short column);

    void mergeWithDown(unsigned short row, unsigned short column);

    void assignCellsInRow();

    unsigned short getUnusedSetNumber();

    void mergeWithDifferentSet(unsigned short column);


    static void seed_random_from_rosc() {
        uint32_t random = 0x811c9dc5;
        uint8_t next_byte = 0;
        volatile uint32_t *rnd_reg = (uint32_t *) (ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

        for (int i = 0; i < 16; i++) {
            for (int k = 0; k < 8; k++) {
                next_byte = (next_byte << 1) | (*rnd_reg & 1);
            }

            random ^= next_byte;
            random *= 0x01000193;
        }

        srand(random);
    }

    static unsigned short getRandNum(short from, short to) {
        return (unsigned short)rand() % (to + 1 - from) + from;
    }
};

#endif //SPA_PICO_MAZE_H
