#include <iostream>
#include "wifinetwork.hpp"

extern "C" void app_main(void)
{
    WIFINETWORK wifi(WIFI_SSID, WIFI_PW);
    wifi.Connect();

    while (1)
    {
        vTaskDelay(1000);
    }
}