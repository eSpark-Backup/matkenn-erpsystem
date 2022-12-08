#include "main.hpp"

Camera *camera;
QRScanner *QR;

extern "C" void app_main(void)
{
    camera = new Camera;
    camera->setup();

    QR = new QRScanner;
    QR->createTask(QR);

    while(1)
    {   
        if(QR->qrDetected())
        {
            char* payload = QR->getPayload();
            ESP_LOGE("PAYLOAD","%s",payload);
            vTaskDelay(2000/portTICK_RATE_MS);
            
            QR->createTask(QR);
        }
        vTaskDelay(10);
    }
}
