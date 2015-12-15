/*
 * internalI2C.h
 *
 * Created: 10/9/2015 11:22:23 PM
 *  Author: smkk
 *
 * This is used only in the variant directory, should not be exported for the user project.
 * It remap the Wire library for the initialization of the ASME and for the user API related to the
 * GPD and SFX component.
 */ 

#include <Arduino.h>
#include "WireIoExt.h"
#include "internalI2C.h"


void internalI2CInit(void){
	WireTemp.begin();
}

byte readRegister(byte _address, byte regToRead)
{
	WireTemp.beginTransmission(_address);
	WireTemp.write(regToRead);
	WireTemp.endTransmission(false); //endTransmission but keep the connection active

	WireTemp.requestFrom(_address, 1); //Ask for 1 byte, once done, bus is released by default

	while(!WireTemp.available()) ; //Wait for the data to come back
	return WireTemp.read(); //Return this one byte
}

// Writes a single byte (dataToWrite) into regToWrite
bool writeRegister(byte _address, byte regToWrite, byte dataToWrite)
{
	WireTemp.beginTransmission(_address);

	if (!WireTemp.write(regToWrite)) {
		return false;
	}
	if (!WireTemp.write(dataToWrite)) {
		return false;
	}

	uint8_t errorNo = WireTemp.endTransmission(); //Stop transmitting
	return (errorNo == 0);
}