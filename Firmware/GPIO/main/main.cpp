#include "main.hpp"

TaskHandle_t gpioTaskHandle;

extern "C" void app_main(void)
{
    ERPSystemIO gpio;
    xTaskCreate(gpioActuation, "gpioActuation", 1024 * 2, &gpio, 8, &gpioTaskHandle);

    while(1)
    {
        vTaskDelay(10/portTICK_RATE_MS);
    }
}
