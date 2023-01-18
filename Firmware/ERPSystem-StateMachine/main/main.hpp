#ifndef MAIN
#define MAIN

#define LGFX_USE_V1

#include <iostream>
#include <stdio.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/****************** Wrappers *******************/
#include "io.hpp"
#include "sntp.hpp"
#include "cloud.hpp"
#include "camera.hpp"
#include "qrscanner.hpp"
#include "wifinetwork.hpp"
/****************** Wrappers *******************/

#endif
