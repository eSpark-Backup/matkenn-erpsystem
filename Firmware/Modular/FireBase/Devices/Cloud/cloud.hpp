#ifndef CLOUD_HPP
#define CLOUD_HPP

/*********************Firebase*******************/
#include "esp_netif.h"
#include "jsoncpp/value.h"
#include "jsoncpp/json.h"
#include "esp_firebase/app.h"
#include "esp_firebase/rtdb.h"
#include "firebase_config.h"

using namespace ESPFirebase;
/*********************Firebase*******************/

class Cloud{
    protected:
    char* API;
    char* dbURL;
    Json::Value new_data;

    public:
    Cloud(char* API, char* dbURL)
    {
        this->API = API;
        this->dbURL = dbURL;
    }
};

class FIREBASE : public Cloud
{
    public:
    FIREBASE(char* API, char* dbURL) : Cloud(API, dbURL) {};
    void sendData(char* data);
};

#endif