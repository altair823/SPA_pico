//
// Created by 김태현 on 2022/02/12.
//

#include "Maze.h"

Maze::Maze(int maxRow, int maxCol) {
    this->maxRow = maxRow;
    this->maxColumn = maxCol;
    location = new Location *[maxCol];
    for (int tc = 0; tc < maxCol; tc++) {
        location[tc] = new Location[maxRow];
        for (int tr = 0; tr < maxRow; ++tr) {
            location[tc][tr].row = tr;
            location[tc][tr].col = tc;

            location[tc][tr].weight[0] = INF;
            location[tc][tr].weight[1] = INF;
            location[tc][tr].weight[2] = INF;
            location[tc][tr].weight[3] = INF;
        }
    }

    previouslyAssignedSetNumber = 0;
    locationSet = new int[maxRow];
    nextLocationSet = new int[maxRow];
    seed_random_from_rosc();
}

Maze::~Maze() {
    for (int i = 0; i < maxColumn; i++) {
        delete location[i];
    }
    delete location;
    delete locationSet;
    delete nextLocationSet;
}

void Maze::openWall(int row, int column, int direction, int weight) {
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

void Maze::mergeRandomly(int column) {
    for (int row = 0; row < this->maxRow; ++row) {
        if (choiceRandomly()) {
            this->mergeWithRight(row, column);
        }
    }
}

bool Maze::choiceRandomly() {
    if (getRandNum(0, 1) == 0) {
        return false;
    } else {
        return true;
    }
}

void Maze::mergeWithRight(int row, int column) {
    // If the right side cell doesn't exist, do nothing.
    if (row + 1 >= maxRow) {
        return;
    }
    // Groups two cells into the same set.
    updateSet(row + 1, row);

    // Open right side wall at the current cell.
    // This is accompanied by opening the left wall in the right cell.
    this->openWall(row, column, RIGHT, generateWeight());
}

void Maze::updateSet(int targetSetRow, int destSetRow) {
    int targetSetValue = locationSet[targetSetRow];
    int destSetValue = locationSet[destSetRow];
    for (int row = targetSetRow - 1; row < maxRow; row++) {
        if (locationSet[row] == targetSetValue) {
            locationSet[row] = destSetValue;
        }
    }
}

int Maze::generateWeight() {
    int weight;
    // The Maximum weight is below (mean*2)
    do {
        weight = getRandNum(WEIGHT_MIN, WEIGHT_MAX);
    } while (weight <= WEIGHT_MIN || weight >= WEIGHT_MAX);
    return weight;
}

void Maze::expandSetsVertical(int column) {
    existingSet.clear();
    int SetStart = 0;
    int SetEnd = 0;
    int currentSet = 0;
    while (true) {
        for (int row = SetStart; row < maxRow; ++row) {
            // If new set is detected,
            if (locationSet[row] != 0 && currentSet == 0){
                // set start point
                SetStart = row;
                currentSet = locationSet[row];
                existingSet.insert(currentSet);
                // delete set value because we don't need it anymore.
                locationSet[row] = 0;
                // If same set is detected,
            } else if (currentSet == locationSet[row]){
                // just delete it.
                locationSet[row] = 0;
                // If different set is detected,
            } else if (currentSet != locationSet[row]){
                // set end point before current row.
                SetEnd = row - 1;
                // But don't delete the set value.
                break;
            }
            if (row == maxRow - 1){
                SetEnd = row;
                break;
            }
        }

        // For every count of vertical expanding,
        //int expandCount = expandCountGen(gen);
        int expandCount = getRandNum(1, SetEnd - SetStart + 1);
        for (; expandCount > 0; expandCount--) {
            int expandRow = getRandNum(SetStart, SetEnd);
            // If new generated row value is already used, generate again.
            // This can be the bottleneck.
            if (nextLocationSet[expandRow] == currentSet) {
                expandCount++;
                continue;
                // Else, expand vertically.
            } else {
                nextLocationSet[expandRow] = currentSet;

                // Also merge two vertical cells in the real maze.
                mergeWithDown(expandRow, column);
            }
        }
        // End point is reached to the maximum, end the loop.
        // And update locationSet to nextLocationSet.
        if (SetEnd == maxRow - 1) {
            for (int i = 0; i < maxRow; i++){
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

void Maze::mergeWithDown(int row, int column) {
    if (column + 1 >= maxColumn){
        return;
    }
    this->openWall(row, column, DOWN, generateWeight());
}

void Maze::makeMaze() {
    //tempMaze->InitializeMaze();

    // Initial inserting. All cells in first row are inserted in different sets.
    for (int i = 0; i < maxRow; ++i) {
        locationSet[i] = i + 1;
    }
    for (int column = 0; column < maxColumn; ++column) {
        mergeRandomly(column);
        expandSetsVertical(column);
        assignCellsInRow();
        // Last row, merge all cells that has different set value.
        if (column == maxColumn - 1) {
            mergeWithDifferentSet(column);
        }
    }
}

void Maze::assignCellsInRow() {
    previouslyAssignedSetNumber = 0;
    for (int i = 0; i < maxRow; i++) {
        if (locationSet[i] == 0) {
            // Assign new set number to cell which does not have one.
            locationSet[i] = getUnusedSetNumber();
        }
    }
}

int Maze::getUnusedSetNumber() {
    // The number of sets is cannot over the maximum number of horizontal cells.
    for (int i = previouslyAssignedSetNumber + 1; i < maxRow + 1; i++){
        // Find unused set number and assign it.
        if (existingSet.find(i) == existingSet.end()){
            previouslyAssignedSetNumber = i;
            return i;
        }
    }
    std::cout<<"Set number assigning error!"<<std::endl;
    exit(1);
}

void Maze::mergeWithDifferentSet(int column) {
    for (int row = 0; row < maxRow - 1; ++row) {
        if (locationSet[row] != locationSet[row + 1]) {
            mergeWithRight(row, column);
        }
    }
}

void Maze::printMaze() const {
    for (int col = 0; col < maxColumn; col++) {
        for (int row = 0; row < maxRow; row++) {
            if (location[col][row].weight[UP] != INF){
                std::cout<<"U";
            } else {std::cout<<"*";}
            if (location[col][row].weight[DOWN] != INF){
                std::cout<<"D";
            }else {std::cout<<"*";}
            if (location[col][row].weight[LEFT] != INF){
                std::cout<<"L";
            }else {std::cout<<"*";}
            if (location[col][row].weight[RIGHT] != INF){
                std::cout<<"R";
            }else {std::cout<<"*";}
            std::cout << " ";
        }
        std::cout<<"\n";
    }
}

Location *Maze::getAdjacentLoc(int row, int col, char dir) const {
    switch (dir) {
        case UP:
            if (col == 0){
                return nullptr;
            }else{
                return &location[col-1][row];
            }
            break;
        case DOWN:
            if (col == maxColumn - 1){
                return nullptr;
            }else{
                return &location[col+1][row];
            }
            break;
        case LEFT:
            if (row == 0){
                return nullptr;
            }else{
                return &location[col][row-1];
            }
            break;
        case RIGHT:
            if (row == maxRow - 1){
                return nullptr;
            }else{
                return &location[col][row+1];
            }
            break;
        default:
            std::cout<<"wrong directiorn!";
            return nullptr;
            break;
    }
}

