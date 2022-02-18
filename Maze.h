/**
 * @file Maze.h
 * @date 2022/02/17
 * @author altair823
 * @version 1.0
 * @brief Maze implementation file.
 */

#ifndef SPA_PICO_MAZE_H
#define SPA_PICO_MAZE_H

#include "pico/stdlib.h"
#include "hardware/structs/rosc.h"

/**
 * @def INF Pseudo-infinite value of the weight.
 * This might be used to representing the closed wall.
 */
/**
 * @def WEIGHT_MAX The maximum value of the weight.
 */
/**
 * @def WEIGHT_MIN The minimum value of the weight.
 */

/*
 * Directions for adjacent locations.
 */
#define UP 3
#define DOWN 2
#define LEFT 1
#define RIGHT 0


#define INF 10000
#define WEIGHT_MAX 5
#define WEIGHT_MIN 1

/**
 * Calculate the absolute value of the distance between from x to y.
 */
#define ABS_MIN_WEIGHT(x, y) ((x->row - y->row) > 0 ? ((x->row - y->row) * WEIGHT_MIN) : -((x->row - y->row) * WEIGHT_MIN))

/**
 * Generates a random number with a range between from and to.
 */
#define GET_RAND_NUM(from, to) ((T) rand() % (to + 1 - from) + from)

/**
 * Location class that makes up the maze.
 * @tparam T The data type of row and column value of the maze.
 * @tparam W The data type of the weights between adjacent locations.
 */
template <typename T, typename W>
struct Location{
    T row;
    T col;
    W weight[4];
};

/**
 * A class that implements a maze with the location class.
 * Using the Eller's algorithm to create a maze.
 * @tparam T The data type of row and column value of the maze.
 * @tparam W The data type of the weights between adjacent locations.
 */
template <typename T, typename W>
struct Maze {
public:

    /**
     * Constructor for the Maze class.
     * @param maxRow Row size of the new maze.
     * @param maxCol Column size of the new maze.
     */
    Maze(T maxRow, T maxCol);

    /**
     * Destructor for the Maze class.
     */
    ~Maze();

    /**
     * Build the maze by opening some percentages of the walls in the maze by using the Eller's algorithm.
     * For more information about the Eller's algorithm, visit <a href="https://altair823.tistory.com/entry/%EB%AF%B8%EB%A1%9C-%EC%83%9D%EC%84%B1-%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98">altair823's blog</a>
     * and <a href="http://weblog.jamisbuck.org/2010/12/29/maze-generation-eller-s-algorithm">The Buckblog</a>.
     */
    void make();

    /**
     * Print wall data of all locations in maze.
     */
    void print() const;

    /**
     * Getter for adjacent location from current location and given direction.
     * @param row Row value of the current location.
     * @param col Column value of the current location.
     * @param dir Direction for the wanted adjacent location.
     * @return The adjacent location pointer.
     */
    Location<T, W> *getAdjacentLoc(T row, T col, char dir) const;

    T maxRow;
    T maxColumn;
    Location<T, W> **location;
private:
    T *locationSet;
    T *nextLocationSet;
    T previouslyAssignedSetNumber;
    bool* existSetNumList;
    void openWall(T row, T column, char direction, W weight);
    void mergeWithRight(T row, T column);
    W generateWeight();
    void expandSetsVertical(T column);
    T getUnusedSetNumber();
};

template<typename T, typename W>
Maze<T, W>::Maze(T maxRow, T maxCol) {
    this->maxRow = maxRow;
    this->maxColumn = maxCol;
    location = new Location<T, W> *[maxCol];
    for (T tc = 0; tc < maxCol; tc++) {
        location[tc] = new Location<T, W>[maxRow];
        for (T tr = 0; tr < maxRow; ++tr) {
            location[tc][tr].row = tr;
            location[tc][tr].col = tc;

            location[tc][tr].weight[0] = INF;
            location[tc][tr].weight[1] = INF;
            location[tc][tr].weight[2] = INF;
            location[tc][tr].weight[3] = INF;
        }
    }

    previouslyAssignedSetNumber = 0;
    locationSet = new T[maxRow];
    nextLocationSet = new T[maxRow];
    existSetNumList = new bool[maxRow];

    // Seeding for random num.
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

template<typename T, typename W>
Maze<T, W>::~Maze() {
    for (T i = 0; i < maxColumn; i++) {
        delete location[i];
    }
    delete[] location;
    delete[] locationSet;
    delete[] nextLocationSet;
    delete[] existSetNumList;
}

template<typename T, typename W>
void Maze<T, W>::make() {
    // Initial inserting. All cells in first row are inserted in different sets.
    for (T i = 0; i < maxRow; ++i) {
        locationSet[i] = i + 1;
    }
    for (T column = 0; column < maxColumn; ++column) {
        for (T r = 0; r < this->maxRow; ++r) {
            if (GET_RAND_NUM(0, 1) == 0) {
                this->mergeWithRight(r, column);
            }
        }
        expandSetsVertical(column);
        previouslyAssignedSetNumber = 0;
        for (T i = 0; i < maxRow; i++) {
            if (locationSet[i] == 0) {
                // Assign new set number to cell which does not have one.
                locationSet[i] = getUnusedSetNumber();
            }
        }
        // Last row, merge all cells that has different set value.
        if (column == maxColumn - 1) {
            for (T row = 0; row < maxRow - 1; ++row) {
                if (locationSet[row] != locationSet[row + 1]) {
                    mergeWithRight(row, column);
                }
            }
        }
    }
}

template<typename T, typename W>
void Maze<T, W>::print() const {
    for (T col = 0; col < maxColumn; col++) {
        for (T row = 0; row < maxRow; row++) {
            if (location[col][row].weight[UP] != INF) {
                std::cout << "U";
            } else { std::cout << "*"; }
            if (location[col][row].weight[DOWN] != INF) {
                std::cout << "D";
            } else { std::cout << "*"; }
            if (location[col][row].weight[LEFT] != INF) {
                std::cout << "L";
            } else { std::cout << "*"; }
            if (location[col][row].weight[RIGHT] != INF) {
                std::cout << "R";
            } else { std::cout << "*"; }
            std::cout << " ";
        }
        std::cout << "\n";
    }
}

template<typename T, typename W>
Location<T, W> *Maze<T, W>::getAdjacentLoc(T row, T col, char dir) const {
    if (location[col][row].weight[dir] >= INF) {
        return nullptr;
    }
    switch (dir) {
        case UP:
            if (col == 0) {
                return nullptr;
            } else {
                return &location[col - 1][row];
            }
        case DOWN:
            if (col == maxColumn - 1) {
                return nullptr;
            } else {
                return &location[col + 1][row];
            }
        case LEFT:
            if (row == 0) {
                return nullptr;
            } else {
                return &location[col][row - 1];
            }
        case RIGHT:
            if (row == maxRow - 1) {
                return nullptr;
            } else {
                return &location[col][row + 1];
            }
        default:
            std::cout << "wrong directiorn!";
            return nullptr;
    }
}

template<typename T, typename W>
void Maze<T, W>::openWall(T row, T column, char direction, W weight) {
    if ((row == 0 && direction == LEFT) ||
        (row == maxRow - 1 && direction == RIGHT) ||
        (column == 0 && direction == UP) ||
        (column == maxColumn - 1 && direction == DOWN)) {
        return;
    }
    // Open the wall in current cell location.
    location[column][row].weight[direction] = weight;

    // Open the wall in corresponding adjacent cell's wall.
    switch (direction) {
        case UP:
            location[column - 1][row].weight[DOWN] = weight;
            break;
        case DOWN:
            location[column + 1][row].weight[UP] = weight;
            break;
        case LEFT:
            location[column][row - 1].weight[RIGHT] = weight;
            break;
        case RIGHT:
            location[column][row + 1].weight[LEFT] = weight;
            break;
        default:
            std::cout << "There is no adjacent cell in " << direction << " direction! (cell row: " << row
                      << ", cell col: " << column << ")" << std::endl;
            exit(1);
    }
}

template<typename T, typename W>
void Maze<T, W>::mergeWithRight(T row, T column) {
    // If the right side cell doesn't exist, do nothing.
    if (row + 1 >= maxRow) {
        return;
    }
    // Groups two cells into the same set.
    T targetSetValue = locationSet[row + 1];
    T destSetValue = locationSet[row];
    for (T i = row; i < maxRow; i++) {
        if (locationSet[i] == targetSetValue) {
            locationSet[i] = destSetValue;
        }
    }
    // Open right side wall at the current cell.
    // This is accompanied by opening the left wall in the right cell.
    this->openWall(row, column, RIGHT, generateWeight());
}

template<typename T, typename W>
W Maze<T, W>::generateWeight() {
    W weight;
    // The Maximum weight is below (mean*2)
    do {
        weight = (W) GET_RAND_NUM(WEIGHT_MIN, WEIGHT_MAX);
    } while (weight <= WEIGHT_MIN || weight >= WEIGHT_MAX);
    return weight;
}

template<typename T, typename W>
void Maze<T, W>::expandSetsVertical(T column) {
    for (int i = 0; i < maxRow; i++) {
        existSetNumList[i] = false;
    }
    T SetStart = 0;
    T SetEnd = 0;
    T currentSet = 0;
    while (true) {
        for (T row = SetStart; row < maxRow; ++row) {
            // If new set is detected,
            if (locationSet[row] != 0 && currentSet == 0) {
                // set start point
                SetStart = row;
                currentSet = locationSet[row];
                existSetNumList[currentSet] = true;
                // delete set value because we don't need it anymore.
                locationSet[row] = 0;
                // If same set is detected,
            } else if (currentSet == locationSet[row]) {
                // just delete it.
                locationSet[row] = 0;
                // If different set is detected,
            } else if (currentSet != locationSet[row]) {
                // set end point before current row.
                SetEnd = row - 1;
                // But don't delete the set value.
                break;
            }
            if (row == maxRow - 1) {
                SetEnd = row;
                break;
            }
        }


        T expandCount = GET_RAND_NUM(1, (T) (SetEnd - SetStart + 1));
        for (; expandCount > 0; expandCount--) {
            T expandRow = GET_RAND_NUM(SetStart, SetEnd);
            // If new generated row value is already used, generate again.
            // This can be the bottleneck.
            if (nextLocationSet[expandRow] == currentSet) {
                expandCount++;
                continue;
                // Else, expand vertically.
            } else {
                nextLocationSet[expandRow] = currentSet;

                // Also merge two vertical cells in the real maze.

                if (column + 1 < maxColumn) {
                    this->openWall(expandRow, column, DOWN, generateWeight());
                }
            }
        }
        // End point is reached to the maximum, end the loop.
        // And update locationSet to nextLocationSet.
        if (SetEnd == maxRow - 1) {
            for (T i = 0; i < maxRow; i++) {
                locationSet[i] = nextLocationSet[i];
                nextLocationSet[i] = 0;
            }
            break;
        } else {
            SetStart = SetEnd + 1;
            currentSet = 0;
        }
    }
}

template<typename T, typename W>
T Maze<T, W>::getUnusedSetNumber() {
    // The number of sets is cannot over the maximum number of horizontal cells.
    for (T i = previouslyAssignedSetNumber + 1; i < maxRow + 1; i++) {
        // Find unused set number and assign it.
        if (existSetNumList[i] == false) {
            previouslyAssignedSetNumber = i;
            return i;
        }
    }
    exit(1);
}

#endif //SPA_PICO_MAZE_H
