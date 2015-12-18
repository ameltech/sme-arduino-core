# sme-arduino-core
Arduino core for Smart Everything board

v1.0.0 First Release
--------------------


v1.1.0 Second Release 18-Dec-2015
---------------------------------
* Add new function to flash the RGB LED light for X milliseconds<br>
    `void flashRGBLed(uint32_t color, uint32_t time_in_ms);`<br><br>
* Added new GPS & SFX commands<br>
    `gpsForceOn() = send an HW wakeup to GPS`<br>
    `sfxSleep()   = put SFX in sleep`<br>
    `sfxWakeup()  = send an HW wakeup to SFX`<br>
