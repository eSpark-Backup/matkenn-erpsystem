#include "qrscanner.hpp"

#define TAG "QRScanner"

void QRScanner::setup()
{
    ESP_LOGI(TAG, "Camera Setup.");
    camera.setup();

    qrRecognizer = NULL;
    fb = NULL;
    image = NULL;
    qrCount = 0;
    oldWidth = 0;
    oldHeight = 0;

    ESP_LOGI(TAG, "Construct a new QR-code recognizer(quirc).");
    qrRecognizer = quirc_new();
    if (qrRecognizer == NULL) 
    {
        ESP_LOGE(TAG, "Can't create quirc object");
    }
}

bool QRScanner::scan()
{
    //Capture Image
    fb = esp_camera_fb_get();
    if(!fb)
    {
        ESP_LOGE(TAG, "Camera capture failed");
        return false;
    }

    //Image Resizing
    if (oldWidth != fb->width || oldHeight != fb->height)
    {
      if (quirc_resize(qrRecognizer, fb->width, fb->height) < 0)
      {
        ESP_LOGE(TAG, "Resize the QR-code recognizer err (cannot allocate memory).");
        esp_camera_fb_return(fb);
        return false;
      }
      else
      {
        oldWidth = fb->width;
        oldHeight = fb->height;
      }
    }

    // QR Identification:
    image = quirc_begin(qrRecognizer, NULL, NULL);
    memcpy(image, fb->buf, fb->len);
    quirc_end(qrRecognizer);
    esp_camera_fb_return(fb);

    int count = quirc_count(qrRecognizer);
    if (count)
    {
        if(dumpInfo(qrRecognizer, count)) {return true;}
    }

    return false;
}

bool QRScanner::dumpInfo(struct quirc *q, uint8_t count)
{
    ESP_LOGI(TAG,"%d QR-codes found", count);
    for (int i = 0; i < count; i++) 
    {
        struct quirc_code code;
        struct quirc_data data;

        // Extract the QR-code specified by the given index.
        quirc_extract(q, i, &code);

        //Decode a QR-code, returning the payload data.
        quirc_decode_error_t err = quirc_decode(&code, &data);

        if (err) 
        {
            ESP_LOGI("Decoding", "FAILED , %s", quirc_strerror(err));
            return false;
        } 
        else 
        {
            ESP_LOGI("Decoding", "%s", "Successfull");
            ESP_LOGI("Payload: ", "%s", data.payload);
            this->payload = (char *)data.payload;

            return true;
        }
    }
    return false;
}

char* QRScanner::getPayload()
{   
    return this->payload; 
}
