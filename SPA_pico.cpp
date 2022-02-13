#include <vector>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "Maze.h"
#include "SPA/DIK.h"
#include "SPA/ASPQ.h"
#include "SPA/ASBQ.h"

#define ABSOLUTE_TIME_BASE 1000000

int main() {
    stdio_init_all();


    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_put(LED_PIN, true);
    uint64_t  pathFindStartTime;
    uint64_t  pathFindEndTime;

    int maxRow = 70;
    int maxCol = 70;
    long counter = 0;
    sleep_ms(5000);
    while (true) {
        counter++;

        std::cout<<"\nCount: " << counter<<std::endl;

        gpio_put(LED_PIN, true);
        uint64_t  mazeMakeStartTime = get_absolute_time()._private_us_since_boot;
        Maze* maze = new Maze(maxRow, maxCol);
        maze->makeMaze();
        uint64_t  mazeMakeEndTime = get_absolute_time()._private_us_since_boot;
        maze->printMaze();
        std::cout<<"Created a new maze in            " << (float )(mazeMakeEndTime - mazeMakeStartTime) / ABSOLUTE_TIME_BASE << " sec."<<std::endl;

        gpio_put(LED_PIN, false);
        sleep_ms(500);
        gpio_put(LED_PIN, true);

//        pathFindStartTime = get_absolute_time()._private_us_since_boot;
//        ASBQ* asbq = new ASBQ(maxRow, maxCol, *maze);
//        asbq->setStart(0, maxCol - 1);
//        asbq->setEnd(maxRow - 1, 0);
//        asbq->findSP();
//        pathFindEndTime = get_absolute_time()._private_us_since_boot;
//        std::cout<<"Shortest path found by "<<asbq->getTypeName()<<" in " << (float )(pathFindEndTime - pathFindStartTime) / ABSOLUTE_TIME_BASE << " sec. "<<std::endl;
//
//        gpio_put(LED_PIN, false);
//        sleep_ms(500);
//        gpio_put(LED_PIN, true);
//
//        delete asbq;

        pathFindStartTime = get_absolute_time()._private_us_since_boot;
        ASPQ* aspq = new ASPQ(maxRow, maxCol, *maze);
        aspq->setStart(0, maxCol - 1);
        aspq->setEnd(maxRow - 1, 0);
        aspq->findSP();
        pathFindEndTime = get_absolute_time()._private_us_since_boot;
        //aspq->printShortestPath();
        std::cout<<"Shortest path found by "<<aspq->getTypeName()<<" in " << (float )(pathFindEndTime - pathFindStartTime) / ABSOLUTE_TIME_BASE << " sec. "<<std::endl;

        gpio_put(LED_PIN, false);
        sleep_ms(500);
        gpio_put(LED_PIN, true);

        delete aspq;

        pathFindStartTime = get_absolute_time()._private_us_since_boot;
        DIK* dik = new DIK(maxRow, maxCol, *maze);
        dik->setStart(0, maxCol - 1);
        dik->setEnd(maxRow - 1, 0);
        dik->findSP();
        pathFindEndTime = get_absolute_time()._private_us_since_boot;
        std::cout<<"Shortest path found by "<<dik->getTypeName()<<" in " << (float )(pathFindEndTime - pathFindStartTime) / ABSOLUTE_TIME_BASE << " sec. "<<std::endl;

        gpio_put(LED_PIN, false);

        delete dik;

        delete maze;
        sleep_ms(1000);
    }
}
