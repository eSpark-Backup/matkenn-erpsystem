#ifndef QRSCANNER
#define QRSCANNER

#include <esp_log.h>
#include <esp_system.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Camera.hpp"
#include "quirc.h"
#include "quirc_internal.h"
#include "dl_lib_matrix3d.h"

#define QR_TASK_DELAY_MS pdMS_TO_TICKS(10)

void QRtask(void* parameters);
void QRJPEGtask(void* parameters);

class QRScanner
{
    private:
    Camera camera;
    char* payload;
    TaskHandle_t qrTaskHandle;

    public:
        struct quirc *qrRecognizer;
        camera_fb_t* fb;
        uint8_t *image;
        uint8_t qrCount;
        bool detect = false;
        uint16_t oldWidth;
        uint16_t oldHeight;
        
        void setup();
        bool scan();
        char* getPayload();
        bool dumpInfo(struct quirc *q, uint8_t count);
};
#endif