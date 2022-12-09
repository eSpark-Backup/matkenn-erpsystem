#include "cloud.hpp"

void FIREBASE::sendData(char* data)
{
    if(clientStatus)
    {
        FirebaseApp app = FirebaseApp(API_KEY);
        RTDB db = RTDB(&app, DATABASE_URL);

        new_data["Payload"] =  data; 
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