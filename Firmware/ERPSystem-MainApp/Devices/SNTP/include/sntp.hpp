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
    void init(void);
    bool setup(void); 
    char* getCurrentTime();
    static void timeSyncNotificationCallBack(struct timeval *tv);
};



#endif