#ifndef IO_HPP
#define IO_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

class IO
{
    private:
    gpio_num_t number;
    gpio_config_t config;

    public:
    // IO(gpio_num_t number) { this->number = number; }
    void setPin(gpio_num_t number);
    bool inputMode();
    bool outputMode();
    void setHIGH();
    void setLOW();
    int  getState();

};

/************ERPSystem Inputs/Outputs************/
// class ERPSystemIO : public IO
// {
//     public:
//     IO led1;
//     IO led2;
//     IO btn1;
//     IO btn2;
//     void setup();
//     void run();
//     void taskDelete(TaskHandle_t * GPIOtaskHandle);
// };
// void gpioActuation(void* parameters);
/************ERPSystem Inputs/Outputs************/
#endif