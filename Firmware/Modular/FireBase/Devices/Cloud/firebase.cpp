#include "cloud.hpp"

void FIREBASE::sendData(char* data)
{
    FirebaseApp app = FirebaseApp(API_KEY);
    RTDB db = RTDB(&app, DATABASE_URL);

    new_data["Payload"] =  data; 
    db.putData("/QRScan", new_data);

}