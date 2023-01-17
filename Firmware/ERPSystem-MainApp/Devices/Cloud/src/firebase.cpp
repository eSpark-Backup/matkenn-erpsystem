#include "cloud.hpp"

void FIREBASE::sendQRData(char* payload, char* timestamp)
{
    if(!clientStatus)
    {
        return;
    }
    else
    {
        FirebaseApp app = FirebaseApp(API_KEY);
        RTDB db = RTDB(&app, DATABASE_URL);

        new_data["Payload"] =  payload; 
        new_data["Time"] = timestamp;
        db.putData("/QRScan", new_data);
    }
    
}

void FIREBASE::startClient()
{
    clientStatus = true;
}

void FIREBASE::stopClient()
{
    clientStatus = false;
}

bool FIREBASE::getClientStatus()
{
    return clientStatus;
}