#include "main.hpp"

extern "C" void app_main(void)
{
    WIFINETWORK wifi;
    wifi.Connect();

    SNTP time;
    time.setSystemTime();

    while (1)
    {
        ESP_LOGI("SNTP","%s",time.getCurrentTime());
        vTaskDelay(1000/portTICK_RATE_MS);
    }
    
}
