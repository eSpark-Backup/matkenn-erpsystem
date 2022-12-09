#include "main.hpp"

extern "C" void app_main(void)
{
    WIFINETWORK wifi;
    wifi.Connect();

    FIREBASE fireBase(API_KEY, DATABASE_URL);
    fireBase.startClient();

    while(1)
    {
        fireBase.sendData("hehe");
        vTaskDelay(1000/portTICK_RATE_MS);
    }
} 
