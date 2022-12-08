#include "main.hpp"

Camera *camera;
QRScanner *QR;
ERPSystemIO gpio;

TaskHandle_t gpioTaskHandle;

extern "C" void app_main(void)
{
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
            vTaskDelay(2000/portTICK_RATE_MS);
            
            QR->createTask(QR);
        }        
        vTaskDelay(10/portTICK_RATE_MS);
    }
}
