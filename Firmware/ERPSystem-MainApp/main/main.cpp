#include "main.hpp"

ERPSystemIO gpio;
SNTP sntp;
WIFINETWORK wifi(WIFI_SSID, WIFI_PW);

TaskHandle_t gpioTaskHandle;

extern "C" void app_main(void)
{
    wifi.Connect();
    FIREBASE fireBase(API_KEY, DATABASE_URL);
    sntp.setSystemTime();

    QRScanner QR;
    QR.createTask();
    
    char* payload;
    char* timestamp;
    
    xTaskCreate(gpioActuation, "gpioActuation", 1024 * 2, &gpio, 8, &gpioTaskHandle);

    while(1)
    {
        if(QR.qrDetected())
        {
            timestamp = sntp.getCurrentTime();
            payload = QR.getPayload();

            fireBase.startClient();
            fireBase.sendQRData(payload,timestamp);
        }
        if(fireBase.getClientStatus())
        {
            fireBase.stopClient();
            QR.createTask();
        }
 
        vTaskDelay(10/portTICK_RATE_MS);
    }
}
