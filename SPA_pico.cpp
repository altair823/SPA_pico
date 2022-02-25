#include <vector>
#include <iostream>
#include <memory>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "Maze.h"
#include "SPA/SPA.h"
#include "SPA/DIK.h"
#include "SPA/ASPQ.h"
#include "SPA/ASBQ.h"

#define ABSOLUTE_TIME_BASE 1000000
#define INTERVAL 50
#define LED_ON      gpio_put(LED_PIN, true)
#define LED_OFF     gpio_put(LED_PIN, false)
#define LED_BLINK   gpio_put(LED_PIN, false); \
                    sleep_ms(INTERVAL); \
                    gpio_put(LED_PIN, true)

int main() {
    stdio_init_all();

    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    LED_ON;
    uint64_t  pathFindStartTime;
    uint64_t  pathFindEndTime;

    unsigned char maxRow = 125;
    unsigned char maxCol = 125;
    long counter = 0;
    sleep_ms(3000);
    
    SPA<unsigned char, unsigned short>* spa;
    while (true) {
        counter++;

        std::cout<<std::fixed;
        std::cout.precision(6);
        std::cout<<"\nCount: " << counter<<std::endl;

        LED_ON;
        uint64_t  mazeMakeStartTime = get_absolute_time()._private_us_since_boot;
        auto maze = new Maze<unsigned char, unsigned short>(maxRow, maxCol);
        maze->make();
        uint64_t  mazeMakeEndTime = get_absolute_time()._private_us_since_boot;
        //maze->print();
        std::cout<<"Created a new maze             in " << (float )(mazeMakeEndTime - mazeMakeStartTime) / ABSOLUTE_TIME_BASE << " sec. size: "<<
        static_cast<unsigned>(maze->maxRow)<<"x"<<static_cast<unsigned>(maze->maxColumn)<<std::endl;

        LED_BLINK;

        for (int i = 0; i < 3; i++){
            pathFindStartTime = get_absolute_time()._private_us_since_boot;
            switch (i){
                case 0:
                    spa = new ASBQ<unsigned char, unsigned short>(maxRow, maxCol, *maze);
                    break;
                case 1:
                    spa = new ASPQ<unsigned char, unsigned short>(maxRow, maxCol, *maze);
                    break;
                case 2:
                    spa = new DIK<unsigned char, unsigned short>(maxRow, maxCol, *maze);
                    break;
                default:
                    break;
            }
            spa->setStart(0, maxCol - 1);
            spa->setEnd(maxRow - 1, 0);
            spa->findSP();
            std::cout<<"Shortest path found by "<<spa->getTypeName()<<"  in " << (float )(pathFindEndTime - pathFindStartTime) / ABSOLUTE_TIME_BASE << " sec. Length: "<<spa->getShortestPathLength()<<std::endl;
            pathFindEndTime = get_absolute_time()._private_us_since_boot;
            LED_BLINK;
            delete spa;
        }

        delete maze;
        LED_OFF;
        sleep_ms(INTERVAL);
    }
}
