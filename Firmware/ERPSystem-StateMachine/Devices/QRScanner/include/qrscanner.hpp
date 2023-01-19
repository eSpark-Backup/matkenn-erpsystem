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

#define QR_TASK_DELAY_MS pdMS_TO_TICKS(5000)

void QRtask(void* parameters);
void QRJPEGtask(void* parameters);

enum qrState
{
    eCameraCapture,
    eImageResizing,
    eQrDetect,
    eQrExtract
};

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
        void loop();
        bool detectQR();
        bool extractQR();
        bool scan();
        bool scanQR();
        bool isDetected();
        char* getPayload();
        void createTask();
        void deleteTask();
        void dumpData(const struct quirc_data *data);
        bool dumpInfo(struct quirc *q, uint8_t count);
        bool jpg2Rgb(camera_fb_t *fb, dl_matrix3du_t **image_matrix);
        size_t grayScale(dl_matrix3du_t *image_888,dl_matrix3du_t **gray);
        
        
};
#endif