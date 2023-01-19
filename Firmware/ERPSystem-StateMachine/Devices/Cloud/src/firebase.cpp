#include "cloud.hpp"

void FIREBASE::sendQRData(char* payload, char* timestamp)
{
    // if(!clientStatus)
    // {
    //     return;
    // }
    // else
    // {
        FirebaseApp app = FirebaseApp(API_KEY);
        RTDB db = RTDB(&app, DATABASE_URL);

        newData["Payload"] =  payload; 
        newData["Time"] = timestamp;
        db.putData("/QRScan", newData);
    // }
    
}

void FIREBASE::startClient()
{
    WIFINETWORK WIFI(SSID, PASSWORD);
    WIFI.connect();
}
