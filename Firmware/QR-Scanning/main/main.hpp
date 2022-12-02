#ifndef MAIN
#define MAIN

#define LGFX_USE_V1
// #define LV_TICK_PERIOD_MS 1

#include <stdio.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_camera.h"
#include "qrcode_recoginize.h"
#include "quirc.h"
#include "JPEGDEC.h"

#include "../Devices/Camera/camera.hpp"

#endif