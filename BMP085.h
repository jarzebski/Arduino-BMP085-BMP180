/*
BMP085.h - Header file for the BMP085/BMP180 Barometric Pressure & Temperature Sensor Arduino Library.

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

#ifndef BMP085_h
#define BMP085_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define BMP085_ADDRESS                (0x77)

#define BMP085_REG_CHIP_ID            (0xD0)
#define BMP085_REG_SOFT_RESET         (0xE0)

#define BMP085_REG_AC1                (0xAA)
#define BMP085_REG_AC2                (0xAC)
#define BMP085_REG_AC3                (0xAE)
#define BMP085_REG_AC4                (0xB0)
#define BMP085_REG_AC5                (0xB2)
#define BMP085_REG_AC6                (0xB4)
#define BMP085_REG_B1                 (0xB6)
#define BMP085_REG_B2                 (0xB8)
#define BMP085_REG_MB                 (0xBA)
#define BMP085_REG_MC                 (0xBC)
#define BMP085_REG_MD                 (0xBE)
#define BMP085_REG_CONTROL            (0xF4)
#define BMP085_REG_DATA               (0xF6)

#define BMP085_CMD_MEASURE_TEMP       (0x2E) // Max conversion time 4.5ms
#define BMP085_CMD_MEASURE_PRESSURE_0 (0x34) // Max conversion time 4.5ms (osrs = 0)
#define BMP085_CMD_MEASURE_PRESSURE_1 (0x74) // Max conversion time 7.5ms (osrs = 1)
#define BMP085_CMD_MEASURE_PRESSURE_2 (0xB4) // Max conversion time 13.5ms (osrs = 2)
#define BMP085_CMD_MEASURE_PRESSURE_3 (0xF4) // Max conversion time 25.5ms (osrs = 3)

typedef enum
{
    BMP085_OSRS_ULTRA_HIGR_RES   = 0x03,
    BMP085_OSRS_HIGH_RES         = 0x02,
    BMP085_OSRS_STANDARD         = 0x01,
    BMP085_OSRS_ULTRA_LOW_POWER  = 0x00
} osrs_t;


class BMP085
{
    public:
	bool begin(void);

    private:

	int16_t ac1;
	int16_t ac2;
	int16_t ac3;
	uint16_t ac4;
	uint16_t ac5;
	uint16_t ac6;
	int16_t b1;
	int16_t b2;
	int16_t mb;
	int16_t mc;
	int16_t md;

	void readCalibrationData(void);

	void writeRegister8(uint8_t reg, uint8_t value);
	uint8_t readRegister8(uint8_t reg);
	uint8_t fastRegister8(uint8_t reg);
	int16_t readRegister16(uint8_t reg);
};

#endif
