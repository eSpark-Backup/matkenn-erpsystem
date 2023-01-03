# ERPSystem-MainAPP
The ERPSystem-MainApp consists of following routines:

1. Wifi connection is build and a firebase cloud is setup.
2. SNTP is initialized to get the timestamp.
3. Camera is setup to perform the QR Scanning.  (Running On Task)
4. 4-GPIO's are setup [2-Buttons] & [2-Leds]    (Running On Task)

# GOAL-TO-ACHIEVE
To Scan a QR and push its payload with the timestamp on the Firebase Cloud. Concurrently the 2-Buttons will change the state of 2-Leds respectively.

# ESP-IDF ENVIRONMENT DETAILS
ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both).
In this `CMakeLists.txt`, the line `set(EXTRA_COMPONENT_DIRS components Devices)` means that the project will also look for library files in both folders.

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt             Defines ESP-IDF project configuration
├── components                 3rd-Party libraries folder
├── Devices                    Wrapper libraries folder        
│   ├── Camera
│   ├── Cloud
│   ├── GPIO
│   ├── QRScanner
│   ├── SNTP
│   ├── Wifi   
├── main
│   ├── CMakeLists.txt          
│   ├── main.cpp               Main Application Code
│   └── main.hpp               Header files for main code are included here. 
└── README.md                  
```
# FIRMWARE FLASHING
Open ESP-IDF terminal, run the following commands:
    - Build :   idf.py build
    - Flash :   idf.py -p COMx flash [check your COM-PORT from the device-manager and replace it by x]
    - Monitor:  idf.py -p COMx monitor 