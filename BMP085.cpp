/*
BMP085.cpp - Class file for the BMP085/BMP180 Barometric Pressure & Temperature Sensor Arduino Library.

Version: 1.0.0
(c) 2014 Korneliusz Jarzebski
www.jarzebski.pl

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include <math.h>
#include <util/delay.h>

#include <BMP085.h>

bool BMP085::begin(oss_t oversampling)
{
    Wire.begin();

    // Check BMP085 REG CHIP_ID
    if (fastRegister8(BMP085_REG_CHIP_ID) != 0x55)
    {
        return false;
    }

    readCalibrationData();

    oss = oversampling;

    return true;
}


// Set oversampling value
void BMP085::setOversampling(oss_t oversampling)
{
    oss = oversampling;
}

// Get oversampling value
oss_t BMP085::getOversampling(void)
{
    return oss;
}

// Read calibration data (BMP085 datasheet, page 13, block 1)
void BMP085::readCalibrationData(void)
{
  ac1 = readRegister16(BMP085_REG_AC1);
  ac2 = readRegister16(BMP085_REG_AC2);
  ac3 = readRegister16(BMP085_REG_AC3);
  ac4 = readRegister16(BMP085_REG_AC4);
  ac5 = readRegister16(BMP085_REG_AC5);
  ac6 = readRegister16(BMP085_REG_AC6);
  b1 = readRegister16(BMP085_REG_B1);
  b2 = readRegister16(BMP085_REG_B2);
  mb = readRegister16(BMP085_REG_MB);
  mc = readRegister16(BMP085_REG_MC);
  md = readRegister16(BMP085_REG_MD);
}

// Read raw temperature (BMP085 datasheet, page 13, block 2)
uint16_t BMP085::readRawTemperature(void)
{
    writeRegister8(BMP085_REG_CONTROL, BMP085_CMD_MEASURE_TEMP);

    _delay_us(4500); // wait 4.5ms

    return readRegister16(BMP085_REG_DATA);
}

// Read raw pressure (BMP085 datasheet, page 13, block 3)
uint32_t BMP085::readRawPressure(void)
{
    uint32_t value;

    writeRegister8(BMP085_REG_CONTROL, BMP085_CMD_MEASURE_PRESSURE_0 + (oss << 6));

    if (oss == BMP085_OSS_ULTRA_LOW_POWER)
    {
	_delay_us(4500);
    } else
    if (oss == BMP085_OSS_STANDARD)
    {
	_delay_us(7500);
    } else
    if (oss == BMP085_OSS_HIGH_RES)
    {
	_delay_us(13500);
    } else
    {
	_delay_us(25500);
    }

    value = readRegister16(BMP085_REG_DATA);
    value <<= 8;
    value |= readRegister8(BMP085_REG_DATA + 2);

    value >>= (8 - oss);

    return value;
}

// Write 8-bit to register
void BMP085::writeRegister8(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(BMP085_ADDRESS);
    #if ARDUINO >= 100
	Wire.write(reg);
	Wire.write(value);
    #else
	Wire.send(reg);
	Wire.send(value);
    #endif
    Wire.endTransmission();
}

// Fast read 8-bit from register
uint8_t BMP085::fastRegister8(uint8_t reg)
{
    uint8_t value;

    Wire.beginTransmission(BMP085_ADDRESS);
    #if ARDUINO >= 100
	Wire.write(reg);
    #else
	Wire.send(reg);
    #endif
    Wire.endTransmission();

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.requestFrom(BMP085_ADDRESS, 1);
    #if ARDUINO >= 100
	value = Wire.read();
    #else
	value = Wire.receive();
    #endif;
    Wire.endTransmission();

    return value;
}

// Read 8-bit from register
uint8_t BMP085::readRegister8(uint8_t reg)
{
    uint8_t value;

    Wire.beginTransmission(BMP085_ADDRESS);
    #if ARDUINO >= 100
	Wire.write(reg);
    #else
	Wire.send(reg);
    #endif
    Wire.endTransmission();

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.requestFrom(BMP085_ADDRESS, 1);
    while(!Wire.available()) {};
    #if ARDUINO >= 100
	value = Wire.read();
    #else
	value = Wire.receive();
    #endif;
    Wire.endTransmission();

    return value;
}

// Read 16-bit from register (oops MSB, LSB)
uint16_t BMP085::readRegister16(uint8_t reg)
{
    uint16_t value;
    Wire.beginTransmission(BMP085_ADDRESS);
    #if ARDUINO >= 100
        Wire.write(reg);
    #else
        Wire.send(reg);
    #endif
    Wire.endTransmission();

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.requestFrom(BMP085_ADDRESS, 2);
    while(!Wire.available()) {};
    #if ARDUINO >= 100
        uint8_t vha = Wire.read();
        uint8_t vla = Wire.read();
    #else
        uint8_t vha = Wire.receive();
        uint8_t vla = Wire.receive();
    #endif;
    Wire.endTransmission();

    value = vha << 8 | vla;

    return value;
}
