#include "qrscanner.hpp"

#define TAG "QRScanner"
#define RGB_LVLS 3
#define RED_LVL 0
#define GREEN_LVL 1
#define BLUE_LVL 2

void QRScanner::createTask(QRScanner* QR)
{
    xTaskCreatePinnedToCore((TaskFunction_t)QRtask, "QRtask", 1024 * 40, (void*) QR, 7, &(this->qrTaskHandle), (BaseType_t)0);
}

void QRScanner::deleteTask()
{
    vTaskDelete((this->qrTaskHandle));
    this->qrTaskHandle = NULL;
}

void QRtask(void* parameters)
{
    QRScanner* QR = (QRScanner*)parameters;

    struct quirc *qrRecognizer;
    camera_fb_t* fb = NULL;
    uint8_t *image = NULL;
    uint16_t oldWidth = 0;
    uint16_t oldHeight = 0;
    int qrCount = 0;

    // Construct a new QR-code recognizer.
    ESP_LOGI(TAG, "Construct a new QR-code recognizer(quirc).");
    qrRecognizer = quirc_new();
    if (!qrRecognizer) { ESP_LOGE(TAG, "Can't create quirc object");}

    vTaskDelay(QR_TASK_DELAY_MS);

    do
    {
        // Capture a frame
        fb = esp_camera_fb_get();
        if (!fb) 
            {
            ESP_LOGE(TAG, "Camera capture failed");
            continue;
        }

        if (oldWidth != fb->width || oldHeight != fb->height) {
            ESP_LOGD(TAG, "Recognizer size change w h len: %d, %d, %d", fb->width, fb->height, fb->len);
            ESP_LOGI(TAG, "Resize the QR-code recognizer.");
            // Resize the QR-code recognizer.
            if (quirc_resize(qrRecognizer, fb->width, fb->height) < 0) {
                ESP_LOGE(TAG, "Resize the QR-code recognizer err.");
                continue;
            } else {
                oldWidth = fb->width;
                oldHeight = fb->height;
            }
        }

        /** These functions are used to process images for QR-code recognition.
         * quirc_begin() must first be called to obtain access to a buffer into
         * which the input image should be placed. Optionally, the current
         * width and height may be returned.
         *
         * After filling the buffer, quirc_end() should be called to process
         * the image for QR-code recognition. The locations and content of each
         * code may be obtained using accessor functions described below.
         */
        image = quirc_begin(qrRecognizer, NULL, NULL);
        memcpy(image, fb->buf, fb->len);
        quirc_end(qrRecognizer);

        // Return the number of QR-codes identified in the last processed image.
        qrCount = quirc_count(qrRecognizer);
        if (qrCount == 0) {
            ESP_LOGE(TAG, "Error: not a valid qrcode");
            esp_camera_fb_return(fb);
            continue;
        }

        // Print information of QR-code
        QR->dumpInfo(qrRecognizer, qrCount);
        esp_camera_fb_return(fb);
    }while(!QR->detect);
    
    // Destroy QR-Code recognizer (quirc)
    quirc_destroy(qrRecognizer);
    qrRecognizer = NULL;
    QR->deleteTask();
}

char* QRScanner::getPayload()
{   
    return this->payload; 
}

bool QRScanner::qrDetected()
{
    bool retVal = this->detect;
    this->detect = false;
    return retVal;
}

void QRScanner::dumpData(const struct quirc_data *data)
{
    ESP_LOGI("Payload: ", "%s", data->payload);
    this->payload = (char *)data->payload;
    this->detect = true;
}


void QRScanner::dumpInfo(struct quirc *q, uint8_t count)
{
    printf("%d QR-codes found:\n\n", count);
    for (int i = 0; i < count; i++) {
        struct quirc_code code;
        struct quirc_data data;

        // Extract the QR-code specified by the given index.
        quirc_extract(q, i, &code);

        //Decode a QR-code, returning the payload data.
        quirc_decode_error_t err = quirc_decode(&code, &data);

        if (err) {
            ESP_LOGI("Decoding", "FAILED , %s", quirc_strerror(err));
            this->detect = false;
        } else {
            ESP_LOGI("Decoding", "%s", "Successfull");
            dumpData(&data);
        }
    }
}


bool QRScanner::jpg2Rgb(camera_fb_t *fb, dl_matrix3du_t **image_matrix)
{
	*image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
    
	if (!*image_matrix)
	{
		printf("dl_matrix3du_alloc failed");
		return false;
	}

	bool s = fmt2rgb888(fb->buf, fb->len, fb->format, (*image_matrix)->item);
    
	if (!s)
	{
		dl_matrix3du_free(*image_matrix);
        image_matrix = NULL;
		printf("to rgb888 failed");
		return false;
	}
	return true;
}

size_t QRScanner::grayScale(dl_matrix3du_t *rgb888Pixels,dl_matrix3du_t **grayScalePixels)
{
    *grayScalePixels = dl_matrix3du_alloc(1, rgb888Pixels->w, rgb888Pixels->h, 1);
	if (!*grayScalePixels)
		return(0);
	(*grayScalePixels)->w = rgb888Pixels->w;
	(*grayScalePixels)->h = rgb888Pixels->h;
	(*grayScalePixels)->c = 1;
	size_t rgbPixelsLen = rgb888Pixels->w * rgb888Pixels->h * rgb888Pixels->c;
	size_t grayPixelsLen = 0;
    uint8_t rgb[RGB_LVLS];

	for(size_t i=0; i < rgbPixelsLen; i+=3 )
	{
		
        rgb[RED_LVL]    = rgb888Pixels->item[i+RED_LVL];
		rgb[GREEN_LVL]  = rgb888Pixels->item[i+GREEN_LVL]; 
		rgb[BLUE_LVL]   = rgb888Pixels->item[i+BLUE_LVL];

        uint8_t maxValue = MAX( MAX(rgb[RED_LVL], rgb[GREEN_LVL]) , rgb[BLUE_LVL]);
		uint8_t minValue = MIN( MIN(rgb[RED_LVL], rgb[GREEN_LVL]) , rgb[BLUE_LVL]);
		uint8_t grayPixel = (maxValue+minValue) >> 1;

		(*grayScalePixels)->item[grayPixelsLen] = grayPixel;
		grayPixelsLen++;	
	}
    return(grayPixelsLen);
}

void QRJPEGtask(void* parameters)
{
    QRScanner* QR = (QRScanner*)parameters;

    struct quirc *qrRecognizer;
    camera_fb_t* fb = NULL;
    uint8_t *image = NULL;
    uint16_t oldWidth = 0;
    uint16_t oldHeight = 0;

    // Construct a new QR-code recognizer.
    ESP_LOGI(TAG, "Construct a new QR-code recognizer(quirc).");
    qrRecognizer = quirc_new();
    if (!qrRecognizer) { ESP_LOGE(TAG, "Can't create quirc object");}

    do
    {
        fb = esp_camera_fb_get();
        if (!fb) 
        {
            ESP_LOGE(TAG, "Camera capture failed");
            return;
        }

        //JPEG to GrayScale:
        dl_matrix3du_t *imageRGB;
        dl_matrix3du_t *imageGray;
        size_t grayPixelsLen;

        QR->jpg2Rgb(fb, &imageRGB);
        grayPixelsLen = QR->grayScale(imageRGB, &imageGray);
        dl_matrix3du_free(imageRGB);
        imageRGB = NULL;
        

        if (oldWidth != imageGray->w|| oldHeight != imageGray->h) {
            ESP_LOGD(TAG, "Recognizer size change w h len: %d, %d, %d", imageGray->w, imageGray->h, grayPixelsLen);
            ESP_LOGI(TAG, "Resize the QR-code recognizer.");
            // Resize the QR-code recognizer.
            if (quirc_resize(qrRecognizer, imageGray->w, imageGray->h) < 0) {
                ESP_LOGE(TAG, "Resize the QR-code recognizer err.");
                return;
            } else {
                oldWidth = imageGray->w;
                oldHeight = imageGray->h;
            }
        }
        

        // ESP_LOGI(TAG, "uxHighWaterMark = %d", uxTaskGetStackHighWaterMark( NULL ));
        image = quirc_begin(qrRecognizer, NULL, NULL);
        // ESP_LOGI(TAG, "uxHighWaterMark = %d", uxTaskGetStackHighWaterMark( NULL ));
        memcpy(image, imageGray->item, grayPixelsLen);
        dl_matrix3du_free(imageGray);
        imageGray = NULL;
        quirc_end(qrRecognizer);
        
        int qrCount = 0;
        // Return the number of QR-codes identified in the last processed image.
        qrCount = quirc_count(qrRecognizer);
        if (qrCount == 0) 
        {
            ESP_LOGE(TAG, "Error: NOT A VALID QRCODE!");
            esp_camera_fb_return(fb);
        }

        // Print information of QR-code 
        QR->dumpInfo(qrRecognizer, qrCount);
        esp_camera_fb_return(fb);
        
     }while(!QR->detect);

    quirc_destroy(qrRecognizer);
    QR->deleteTask();
}