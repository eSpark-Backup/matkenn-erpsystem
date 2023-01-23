#ifndef CLOUD_HPP
#define CLOUD_HPP

/*********************Firebase*******************/
#include "esp_netif.h"
#include "jsoncpp/value.h"
#include "jsoncpp/json.h"
#include "esp_firebase/app.h"
#include "esp_firebase/rtdb.h"
#include "firebase_config.h"
#include "WIFINETWORK.hpp"

using namespace ESPFirebase;
/*********************Firebase*******************/

class Cloud{
    protected:
    char* API;
    char* dbURL;
    Json::Value newData;

    public:
    Cloud(char* API, char* dbURL)
    {
        this->API = API;
        this->dbURL = dbURL;
    }
};

class FIREBASE : public Cloud
{
    private:
    bool clientStatus = false;

    public:
    FIREBASE(char* API, char* dbURL) : Cloud(API, dbURL) {};
    void sendQRData(char* payload, char* timestamp = "xx:xx:xx");
    void startClient();
};

#endif