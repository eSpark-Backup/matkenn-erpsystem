#include "io.hpp"

#define BTN1 GPIO_NUM_12
#define BTN2 GPIO_NUM_13
#define LED1 GPIO_NUM_14
#define LED2 GPIO_NUM_4

void ERPSystemIO::setup()
{
    led1.setPin((gpio_num_t)LED1);
    led2.setPin((gpio_num_t)LED2);
    btn1.setPin((gpio_num_t)BTN1);
    btn2.setPin((gpio_num_t)BTN2);
    
    led1.outputMode();
    led2.outputMode();
    btn1.inputMode();
    btn2.inputMode();

}

void ERPSystemIO::run()
{
    led1.setState(btn1.getState());
    led2.setState(btn2.getState());
}

void ERPSystemIO::taskDelete(TaskHandle_t * GPIOtaskHandle)
{
    led1.setLOW();
    led2.setLOW();
    vTaskDelete(*GPIOtaskHandle);
    *GPIOtaskHandle = NULL;
}

void gpioActuation(void* parameters)
{
    ERPSystemIO* gpio = (ERPSystemIO *)parameters;
    gpio->setup();

    while(1)
    {
        gpio->run();
        vTaskDelay(GPIO_TSK_DLY_MS);
    }
}