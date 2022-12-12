#include "main.hpp"

Camera *camera;
QRScanner *QR;
ERPSystemIO gpio;
SNTP QRtime;
WIFINETWORK wifi;

TaskHandle_t gpioTaskHandle;

extern "C" void app_main(void)
{
    wifi.Connect();
    FIREBASE fireBase(API_KEY, DATABASE_URL);
    QRtime.setSystemTime();

    camera = new Camera;
    camera->setup();

    QR = new QRScanner;
    QR->createTask(QR);

    char* payload;
    char* timestamp;
    
    xTaskCreate(gpioActuation, "gpioActuation", 1024 * 2, &gpio, 8, &gpioTaskHandle);

    while(1)
    {
        if(QR->qrDetected())
        {
            timestamp = QRtime.getCurrentTime();
            payload = QR->getPayload();

            fireBase.startClient();
            fireBase.sendQRData(payload,timestamp);
        }
        if(fireBase.getClientStatus())
        {
            fireBase.stopClient();
            QR->createTask(QR);
        }
 
        vTaskDelay(10/portTICK_RATE_MS);
    }
}
