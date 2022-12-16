#include <iostream>
#include "io.hpp"

IO* led1;

extern "C" void app_main(void)
{
    led1 = new IO;
    led1->setPin(GPIO_NUM_14);
    led1->outputMode();

    while(1)
    {
        led1->setHIGH();
        vTaskDelay(1000);

        led1->setLOW();
        vTaskDelay(1000);
    }
}
