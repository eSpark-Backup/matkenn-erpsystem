#include "qrscanner.hpp"

#define TAG "QRScanner"

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

    struct quirc *qr_recognizer;
    camera_fb_t* fb = NULL;
    uint8_t *image = NULL;
    uint16_t old_width = 0;
    uint16_t old_height = 0;
    int id_count = 0;

    // Construct a new QR-code recognizer.
    ESP_LOGI(TAG, "Construct a new QR-code recognizer(quirc).");
    qr_recognizer = quirc_new();
    if (!qr_recognizer) { ESP_LOGE(TAG, "Can't create quirc object");}

    do
    {
        // Capture a frame
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
            continue;
        }

        if (old_width != fb->width || old_height != fb->height) {
            ESP_LOGD(TAG, "Recognizer size change w h len: %d, %d, %d", fb->width, fb->height, fb->len);
            ESP_LOGI(TAG, "Resize the QR-code recognizer.");
            // Resize the QR-code recognizer.
            if (quirc_resize(qr_recognizer, fb->width, fb->height) < 0) {
                ESP_LOGE(TAG, "Resize the QR-code recognizer err.");
                continue;
            } else {
                old_width = fb->width;
                old_height = fb->height;
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
        image = quirc_begin(qr_recognizer, NULL, NULL);
        memcpy(image, fb->buf, fb->len);
        quirc_end(qr_recognizer);

        // Return the number of QR-codes identified in the last processed image.
        id_count = quirc_count(qr_recognizer);
        if (id_count == 0) {
            ESP_LOGE(TAG, "Error: not a valid qrcode");
            esp_camera_fb_return(fb);
            continue;
        }

        // Print information of QR-code
        QR->dump_info(qr_recognizer, id_count);
        esp_camera_fb_return(fb);
    }while(!QR->detect);
    
    // Destroy QR-Code recognizer (quirc)
    quirc_destroy(qr_recognizer);
    qr_recognizer = NULL;
    QR->deleteTask();
}

char* QRScanner::getPayload()
{   
    return this->payload; 
}

bool QRScanner::qrDetected()
{
    if(this->detect == true)
    {
        this->detect = false;
        return true;
    }
    else
    {
        return false;
    }
}

void QRScanner::dump_data(const struct quirc_data *data)
{
    ESP_LOGI("Payload: ", "%s", data->payload);
    this->payload = (char *)data->payload;
    this->detect = true;
}


void QRScanner::dump_info(struct quirc *q, uint8_t count)
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
            dump_data(&data);
        }
    }
}


bool QRScanner::jpg2rgb(camera_fb_t *fb, dl_matrix3du_t **image_matrix)
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

size_t QRScanner::grayScale(dl_matrix3du_t *image_888,dl_matrix3du_t **gray)
{
    *gray = dl_matrix3du_alloc(1,image_888->w,image_888->h,1);
	if (!*gray)
		return(0);
	(*gray)->w = image_888->w;
	(*gray)->h = image_888->h;
	(*gray)->c = 1;
	size_t len = image_888->w * image_888->h * image_888->c;
	size_t lengray=0;
	for(size_t i=0;i<len;i+=3)
	{
		uint8_t r = image_888->item[i];
		uint8_t g = image_888->item[i+1]; 
		uint8_t b = image_888->item[i+2];
		
        uint8_t m = MAX(r, g);
		m = MAX(m,b);
		uint8_t n = MIN(r, g);
		n = MIN(n,b);
		uint8_t gr = (m+n)/2;

		(*gray)->item[lengray]=gr;
		lengray++;	
	}
    return(lengray);
}

void QRJPEGtask(void* parameters)
{
    QRScanner* QR = (QRScanner*)parameters;

    struct quirc *qr_recognizer;
    camera_fb_t* fb = NULL;
    uint8_t *image = NULL;
    uint16_t old_width = 0;
    uint16_t old_height = 0;

    // Construct a new QR-code recognizer.
    ESP_LOGI(TAG, "Construct a new QR-code recognizer(quirc).");
    qr_recognizer = quirc_new();
    if (!qr_recognizer) { ESP_LOGE(TAG, "Can't create quirc object");}

    do
    {
        fb = esp_camera_fb_get();
        if (!fb) 
        {
            ESP_LOGE(TAG, "Camera capture failed");
            return;
        }

        //JPEG to GrayScale:
        dl_matrix3du_t *image_rgb;
        dl_matrix3du_t *image_gray;
        size_t lengray;

        QR->jpg2rgb(fb, &image_rgb);
        lengray = QR->grayScale(image_rgb, &image_gray);
        dl_matrix3du_free(image_rgb);
        image_rgb = NULL;
        

        if (old_width != image_gray->w|| old_height != image_gray->h) {
            ESP_LOGD(TAG, "Recognizer size change w h len: %d, %d, %d", image_gray->w, image_gray->h, lengray);
            ESP_LOGI(TAG, "Resize the QR-code recognizer.");
            // Resize the QR-code recognizer.
            if (quirc_resize(qr_recognizer, image_gray->w, image_gray->h) < 0) {
                ESP_LOGE(TAG, "Resize the QR-code recognizer err.");
                return;
            } else {
                old_width = image_gray->w;
                old_height = image_gray->h;
            }
        }
        

        // ESP_LOGI(TAG, "uxHighWaterMark = %d", uxTaskGetStackHighWaterMark( NULL ));
        image = quirc_begin(qr_recognizer, NULL, NULL);
        // ESP_LOGI(TAG, "uxHighWaterMark = %d", uxTaskGetStackHighWaterMark( NULL ));
        memcpy(image, image_gray->item, lengray);
        dl_matrix3du_free(image_gray);
        image_gray = NULL;
        quirc_end(qr_recognizer);
        
        int id_count = 0;
        // Return the number of QR-codes identified in the last processed image.
        id_count = quirc_count(qr_recognizer);
        if (id_count == 0) 
        {
            ESP_LOGE(TAG, "Error: NOT A VALID QRCODE!");
            esp_camera_fb_return(fb);
        }

        // Print information of QR-code 
        QR->dump_info(qr_recognizer, id_count);
        esp_camera_fb_return(fb);
        
     }while(!QR->detect);

    quirc_destroy(qr_recognizer);
    QR->deleteTask();
}