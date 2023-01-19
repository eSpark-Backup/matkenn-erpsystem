#include "main.hpp"

TaskHandle_t gpioTaskHandle;

extern "C" void app_main(void)
{
    FIREBASE fireBase(API_KEY, DATABASE_URL);
    fireBase.startClient();

    SNTP sntp;
    sntp.setup();

    QRScanner* QR = new QRScanner;
    QR->setup();

    ERPSystemIO gpio;
    xTaskCreate(gpioActuation, "gpioActuation", 1024 * 2, &gpio, 8, &gpioTaskHandle);

    char* payload;
    char* timestamp;
    
    while(1)
    {
        if(QR->scan())
        {
            payload = QR->getPayload();
            timestamp = sntp.getCurrentTime();

            fireBase.sendQRData(payload, timestamp);
        }
    }
}