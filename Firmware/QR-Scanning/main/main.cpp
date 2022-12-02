#include "main.hpp"

Camera *camera;
JPEGDEC jpeg;

extern "C" void app_main(void)
{
    camera = new Camera;

    camera->setup();

    app_qr_recognize(camera->getConfig());

    while (1)
    {
        vTaskDelay(100);
    }
}