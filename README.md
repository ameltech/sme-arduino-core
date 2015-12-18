# sme-arduino-core
Arduino core for Smart Everything board

v1.0.0 First Release
--------------------

v1.1.0 Second Release 18-Dec-2015
---------------------------------
* Add new function to flash the RGB LED light for X milliseconds
    void flashRGBLed(uint32_t color, uint32_t time_in_ms);
* Added new GPS & SFX commands
    gpsForceOn() = send an HW wakeup to GPS
    sfxSleep()   = put SFX in sleep
    sfxWakeup()  = send an HW wakeup to SFX
