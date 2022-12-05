#include "main.hpp"
#include "esp_sntp.h"
#include <time.h>
#include <sys/time.h>
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_sntp.h"

Camera *camera;
JPEGDEC jpeg;

/*********************QR Scanner*******************/
bool qrDetect = 0;
char* qrPayload;
/*********************QR Scanner*******************/

/*********************GPIO*******************/
typedef struct
{
    gpio_num_t led1;
    gpio_num_t led2;
    gpio_num_t btn1;
    gpio_num_t btn2;
} gpioData;

TaskHandle_t gpioTaskHandle;
void gpioToggle(void* parameters);
/*********************GPIO*******************/

/*********************Firebase*******************/
using namespace ESPFirebase;
/*********************Firebase*******************/

void time_sync_notification_cb(struct timeval *tv);
static void initialize_sntp(void);
static void obtain_time(void);

void setTimeStamp(time_t* now, struct tm* timeinfo, char* strftime_buf);
char* getTimeStamp(time_t* now, struct tm* timeinfo, char* strftime_buf);

extern "C" void app_main(void)
{
    camera = new Camera;
    camera->setup();
    app_qr_recognize(camera->getConfig());

    /*********************Firebase*******************/
    wifiInit(SSID, PASSWORD);  // blocking until it connects
    Json::Value new_data; 
    bool putData = 0;
    /*********************Firebase*******************/

    /*********************GPIO*******************/
    gpioData gpio = {
        .led1 = GPIO_NUM_2,
        .led2 = GPIO_NUM_4,
        .btn1 = GPIO_NUM_14,
        .btn2 = GPIO_NUM_15
    };
    xTaskCreate(gpioToggle, "gpioToggle", 1024 * 2, &gpio, 8, &gpioTaskHandle);
    /*********************GPIO*******************/

    // initialize_sntp();
    time_t now;
    struct tm timeinfo;

    char strftime_buf[64];

    while(1)
    {
        if(qrDetect == 1)
        {
            ESP_LOGI("APP:","FireBase Database Initialized");
            FirebaseApp app = FirebaseApp(API_KEY);
            RTDB db = RTDB(&app, DATABASE_URL);

            setTimeStamp(&now, &timeinfo, strftime_buf);
            char* timestamp = getTimeStamp(&now, &timeinfo, strftime_buf);
            ESP_LOGI("TimeStamp","%s",timestamp);

            new_data["Payload"] =  qrPayload; 
            new_data["TimeStamp"] =  timestamp; 
            db.putData("/QRScan", new_data);

            putData = 1;
            
        }
        if(putData == 1)
        {
            ESP_LOGI("APP:","QR-Code Scanner Initialized");
            qrDetect = 0;
            putData = 0;
            app_qr_recognize(camera->getConfig());
            xTaskCreate(gpioToggle, "gpioToggle", 1024 * 2, &gpio, 8, &gpioTaskHandle);
        }
        vTaskDelay(10);
    }

}

void setTimeStamp(time_t* now, struct tm* timeinfo, char* strftime_buf)
{
    setenv("TZ", "GMT-5", 1);
    tzset();
    time(now);
    localtime_r(now, timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo->tm_year < (2022 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();
        // update 'now' variable with current time
        time(now);
    }
}

char* getTimeStamp(time_t* now, struct tm* timeinfo, char* strftime_buf)
{
    localtime_r(now, timeinfo);
    strftime(strftime_buf, 64, "%c", timeinfo);
    sprintf(strftime_buf,"%d:%d:%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    ESP_LOGI(TAG, "The current date/time in Pakistan is: %s", strftime_buf);
    return strftime_buf;
}

void gpioToggle(void* parameters)
{
    gpioData* gpio = (gpioData *)parameters;

    gpio_set_direction(gpio->led1, GPIO_MODE_OUTPUT);
    gpio_set_direction(gpio->led2, GPIO_MODE_OUTPUT);

    gpio_set_direction(gpio->btn1, GPIO_MODE_INPUT);
    gpio_set_direction(gpio->btn2, GPIO_MODE_INPUT);


    while (qrDetect != 1)
    {
        if(gpio_get_level(gpio->btn1) == 0)
        {
            ESP_LOGI("GPIO","LED1 IS ON");
            gpio_set_level(gpio->led1, 1);
        }
        else
        {
            gpio_set_level(gpio->led1, 0);
        }
        if(gpio_get_level(gpio->btn2) == 0)
        {
            ESP_LOGI("GPIO","LED2 IS ON");
            gpio_set_level(gpio->led2, 1);
        }
        else
        {
            gpio_set_level(gpio->led2, 0);
        }

        vTaskDelay(10);
    }
    vTaskDelete(gpioTaskHandle); 
}

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

static void obtain_time(void)
{
    initialize_sntp();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}

