/**
 * @file SPA.h
 * @date 2022/02/17
 * @author altair823
 * @version 1.0
 * @brief Interface to an implementation of an algorithm that finds the shortest path in a given maze.
 */

#ifndef SPA_PICO_SPA_H
#define SPA_PICO_SPA_H

template <typename T, typename W>
class SPA {
protected:

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

public:
    /**
     * Constructor for SPA.
     * @param _maze The reference variable for the maze.
     */
    explicit SPA(Maze<T, W> &_maze) : maze(_maze){}

    /**
     * Setter for the starting point of the maze.
     * @param row A row value of the starting point.
     * @param column A column value of the starting point.
     */
    void setStart(T row, T column) {this->start = &(this->maze.location[column][row]);}

    /**
     * Setter for the ending point of the maze.
     * @param row A row value of the ending point.
     * @param column A column value of the ending point.
     */
    void setEnd(T row, T column) {this->end = &(this->maze.location[column][row]);}

    /**
     * Find the shortest path from the starting point to the ending point in the maze.
     */
    virtual void findSP() = 0;

    /**
     * Getter for the length of the shortest path found.
     * @return The length of the shortest path found.
     */
    virtual W getShortestPathLength() const = 0;

    /**
     * Getter for the name of the algorithm for finding the shortest path in a maze.
     * @return A name of the current algorithm.
     */
    [[nodiscard]] virtual std::string getTypeName() const = 0;
};


#endif //SPA_PICO_SPA_H
