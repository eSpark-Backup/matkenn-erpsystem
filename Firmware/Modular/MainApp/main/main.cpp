#include "main.hpp"

Camera *camera;
QRScanner *QR;
ERPSystemIO gpio;

TaskHandle_t gpioTaskHandle;

extern "C" void app_main(void)
{
    WIFINETWORK wifi;
    wifi.Connect();
    FIREBASE fireBase(API_KEY, DATABASE_URL);

    camera = new Camera;
    camera->setup();

    QR = new QRScanner;
    QR->createTask(QR);

    xTaskCreate(gpioActuation, "gpioActuation", 1024 * 2, &gpio, 8, &gpioTaskHandle);

    while(1)
    {
        if(QR->qrDetected())
        {
            char* payload = QR->getPayload();
            ESP_LOGE("PAYLOAD","%s",payload);
            fireBase.startClient();
            fireBase.sendData(payload);
        }
        if(fireBase.getClientStatus())
        {
            fireBase.stopClient();
            QR->createTask(QR);
            ESP_LOGI("QR","TASK CREATED");
        }
 
        vTaskDelay(10/portTICK_RATE_MS);
    }
}
