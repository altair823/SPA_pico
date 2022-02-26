#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "Maze.h"
#include "SPA/SPA.h"
#include "SPA/DIK.h"
#include "SPA/ASPQ.h"
#include "SPA/ASBQ.h"
#include "lcd.h"

#define ABSOLUTE_TIME_BASE 1000000
#define INTERVAL 50
#define LED_ON      gpio_put(LED_PIN, true)
#define LED_OFF     gpio_put(LED_PIN, false)
#define LED_BLINK   gpio_put(LED_PIN, false); \
                    sleep_ms(INTERVAL); \
                    gpio_put(LED_PIN, true)

int main() {
    stdio_init_all();
    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    lcd_init();

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
        if (counter > 99999){
            counter = 1;
        }

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
            pathFindEndTime = get_absolute_time()._private_us_since_boot;
            auto executionTime = (float )(pathFindEndTime - pathFindStartTime) / ABSOLUTE_TIME_BASE;
            std::cout<<"Shortest path found by "<<spa->getTypeName()<<"  in " << executionTime << " sec. Length: "<<spa->getShortestPathLength()<<std::endl;
            
            lcd_set_cursor(0, 0);

            char spaTypeName[16];
            std::string buffer = "SPA: " + spa->getTypeName() + ", " + std::to_string(counter);
            strcpy(spaTypeName, buffer.c_str());
            lcd_string(spaTypeName);

            std::string executionTimeStr = "time: ";
            executionTimeStr = executionTimeStr + std::to_string(executionTime);
            char executionTimeChar[16];
            strcpy(executionTimeChar, executionTimeStr.c_str());
            executionTimeChar[15] = '\0';
            lcd_set_cursor(1, 0);
            lcd_string(executionTimeChar);
            sleep_ms(1000);
            //lcd_clear();

            LED_BLINK;
            delete spa;
        }

        delete maze;
        LED_OFF;
        sleep_ms(INTERVAL);
    }
}
