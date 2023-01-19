#include "main.hpp"

extern "C" void app_main(void)
{
    WIFINETWORK wifi(WIFI_SSID, WIFI_PW);
    wifi.Connect();

    FIREBASE fireBase(API_KEY, DATABASE_URL);
    fireBase.startClient();

    SNTP sntp;
    sntp.setSystemTime();

    QRScanner* QR = new QRScanner;
    QR->setup();

    char* payload, timestamp;

    while(1)
    {
        QR->scan();
        if(QR->isDetected())
        {
            timestamp = sntp.getCurrentTime();
            payload = QR->getPayload();

            fireBase.sendQRData(payload, timestamp);
        }
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}