#ifndef SNTP_HPP
#define SNTP_HPP

#include <time.h>
#include <sys/time.h>
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_sntp.h"
#include "esp_log.h"
#include "string.h"

class SNTP
{
    private:
    char buffer[64];

    public:
    char* getCurrentTime();
    void setSystemTime();
};


static void initialize_sntp(void);
static void obtain_time(void);
void time_sync_notification_cb(struct timeval *tv);



#endif