#include "sntp.hpp"

#define TAG "SNTP"
#define SNTP_SYNC_DELAY pdMS_TO_TICKS(2000)

void SNTP::init(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(timeSyncNotificationCallBack);
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
    sntp_init();
}

bool SNTP::setup(void)
{
    init();
    int retry = 0;
    do
    {
        sntp_sync_status_t syncStatus = sntp_get_sync_status();
        if(syncStatus == SNTP_SYNC_STATUS_COMPLETED)
            return true;
        
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d)", retry);
        retry++;
        vTaskDelay(SNTP_SYNC_DELAY);
    } while (retry < 20);

    return false;
}

void SNTP::timeSyncNotificationCallBack(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

char* SNTP::getCurrentTime()
{
	char timeBuffer[64];
	time_t now;
	struct tm timeInfo;

	time(&now);
	localtime_r(&now, &timeInfo);

	// Set timezone to Pakistan Standard Time
	setenv("TZ", "GMT-5", 1);
	tzset();
	localtime_r(&now, &timeInfo);

	strftime(timeBuffer, sizeof(timeBuffer), "%c", &timeInfo);
	ESP_LOGI(TAG, "The current date/time in Pakistan is: %s", timeBuffer);
    strcpy(buffer,timeBuffer);
    // sprintf(timeBuffer,"%d:%d:%d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    return buffer;
}
