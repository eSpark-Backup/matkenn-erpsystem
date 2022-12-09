#include "main.hpp"

extern "C" void app_main(void)
{
    WIFINETWORK wifi;
    wifi.Connect();

    SNTP time;
    

    while (1)
    {
        time.setTimeStamp();
        ESP_LOGI("SNTP","%s",time.getTimeStamp());
        vTaskDelay(1000/portTICK_RATE_MS);
    }
    
}
