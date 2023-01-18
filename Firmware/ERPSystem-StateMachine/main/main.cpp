#include "main.hpp"

extern "C" void app_main(void)
{
    QRScanner* QR = new QRScanner;
    QR->setup();
    while(1)
    {
        QR->scan();
        vTaskDelay(10/portTICK_RATE_MS);
    }
}
