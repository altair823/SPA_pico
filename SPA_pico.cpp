#include <vector>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "Maze.h"
#include "SPA/DIK.h"
#include "SPA/ASPQ.h"
#include "SPA/ASBQ.h"

#define ABSOLUTE_TIME_BASE 1000000
#define LED_ON      gpio_put(LED_PIN, true)
#define LED_OFF     gpio_put(LED_PIN, false)
#define LED_BLINK   gpio_put(LED_PIN, false); \
                    sleep_ms(100); \
                    gpio_put(LED_PIN, true)

int main() {
    stdio_init_all();

    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    LED_ON;
    uint64_t  pathFindStartTime;
    uint64_t  pathFindEndTime;

    unsigned char maxRow = 119;
    unsigned char maxCol = 119;
    long counter = 0;
    sleep_ms(3000);
    while (true) {
        counter++;

        std::cout<<std::fixed;
        std::cout.precision(6);
        std::cout<<"\nCount: " << counter<<std::endl;

        LED_ON;
        uint64_t  mazeMakeStartTime = get_absolute_time()._private_us_since_boot;
        auto maze = new Maze<unsigned char, unsigned short>(maxRow, maxCol);
        maze->makeMaze();
        uint64_t  mazeMakeEndTime = get_absolute_time()._private_us_since_boot;
        //maze->printMaze();
        std::cout<<"Created a new maze             in " << (float )(mazeMakeEndTime - mazeMakeStartTime) / ABSOLUTE_TIME_BASE << " sec. size: "<<
        static_cast<unsigned>(maze->maxRow)<<"x"<<static_cast<unsigned>(maze->maxColumn)<<std::endl;

        LED_BLINK;

        pathFindStartTime = get_absolute_time()._private_us_since_boot;
        auto asbq = new ASBQ<unsigned char, unsigned short>(maxRow, maxCol, *maze);
        asbq->setStart(0, maxCol - 1);
        asbq->setEnd(maxRow - 1, 0);
        asbq->findSP();
        pathFindEndTime = get_absolute_time()._private_us_since_boot;
        std::cout<<"Shortest path found by "<<asbq->getTypeName()<<"  in " << (float )(pathFindEndTime - pathFindStartTime) / ABSOLUTE_TIME_BASE <<
        " sec. Length: "<<asbq->getShortestPathLength()<<std::endl;

        LED_BLINK;

        delete asbq;

        pathFindStartTime = get_absolute_time()._private_us_since_boot;
        auto aspq = new ASPQ<unsigned char, unsigned short>(maxRow, maxCol, *maze);
        aspq->setStart(0, maxCol - 1);
        aspq->setEnd(maxRow - 1, 0);
        aspq->findSP();
        pathFindEndTime = get_absolute_time()._private_us_since_boot;
        //aspq->printShortestPath();
        std::cout<<"Shortest path found by "<<aspq->getTypeName()<<"  in " << (float )(pathFindEndTime - pathFindStartTime) / ABSOLUTE_TIME_BASE <<
        " sec. Length: "<<aspq->getShortestPathLength()<<std::endl;

        LED_BLINK;

        delete aspq;

        pathFindStartTime = get_absolute_time()._private_us_since_boot;
        auto dik = new DIK<unsigned char, unsigned short>(maxRow, maxCol, *maze);
        dik->setStart(0, maxCol - 1);
        dik->setEnd(maxRow - 1, 0);
        dik->findSP();
        pathFindEndTime = get_absolute_time()._private_us_since_boot;
        std::cout<<"Shortest path found by "<<dik->getTypeName()<<"  in " << (float )(pathFindEndTime - pathFindStartTime) / ABSOLUTE_TIME_BASE <<
        " sec. Length: "<<dik->getShortestPathLength()<<std::endl;

        gpio_put(LED_PIN, false);

        delete dik;

        delete maze;
        LED_OFF;
        sleep_ms(100);
    }
}
