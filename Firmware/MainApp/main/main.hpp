#ifndef MAIN
#define MAIN

#define LGFX_USE_V1
// #define LV_TICK_PERIOD_MS 1

#include <iostream>
#include <stdio.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*********************QR Scanner*******************/
#include "esp_camera.h"
#include "qrcode_recoginize.h"
#include "quirc.h"
#include "JPEGDEC.h"
/*********************QR Scanner*******************/

/*********************Firebase*******************/
#include "esp_netif.h"
#include "jsoncpp/value.h"
#include "jsoncpp/json.h"
#include "esp_firebase/app.h"
#include "esp_firebase/rtdb.h"
#include "wifi_utils.h"
#include "firebase_config.h"
/*********************Firebase*******************/

#include "../Devices/Camera/camera.hpp"

#endif