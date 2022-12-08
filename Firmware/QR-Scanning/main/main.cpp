#include "main.hpp"

Camera *camera;
QRScanner *QR;
JPEGDEC jpeg;

// void* QRtask(void* parameters);

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
            QR->createTask(QR);
        }
        vTaskDelay(10);
    }
}
