#include <iostream>
#include "camera.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

Camera* cam;
extern "C" void app_main(void)
{
    cam = new Camera;
    cam->setup();
    while(1)
    {
        vTaskDelay(1000);
    }
}
