#include "main.hpp"

/*********************Firebase*******************/
using namespace ESPFirebase;
/*********************Firebase*******************/

extern "C" void app_main(void)
{

    /*********************Firebase*******************/
    wifiInit(SSID, PASSWORD);  // blocking until it connects

    FirebaseApp app = FirebaseApp(API_KEY);

    RTDB db = RTDB(&app, DATABASE_URL);

    // We can put a json str directly at /person1
    std::string json_str = R"({"Name": "JackSparrow", "Age": "20", "Status": "Single"})";
    db.putData("/person1", json_str.c_str()); 
    /*********************Firebase*******************/
}