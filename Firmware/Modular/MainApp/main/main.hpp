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

/*********************QR Scanner*******************/
// #include "esp_camera.h"
// #include "quirc.h"
// #include "JPEGDEC.h"
/*********************QR Scanner*******************/

#include "../Devices/SNTP/sntp.hpp"
#include "../Devices/Camera/camera.hpp"
#include "../Devices/QRScanner/qrscanner.hpp"
#include "../Devices/GPIO/io.hpp"
#include "../Devices/Wifi/wifinetwork.hpp"
#include "../Devices/Cloud/cloud.hpp"


#endif
