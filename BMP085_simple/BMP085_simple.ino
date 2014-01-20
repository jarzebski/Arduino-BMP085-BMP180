/*
    BMP085 / BMP180 Barometric Pressure & Temperature Sensor. Simple Example.
    Read more: TODO
    GIT: https://github.com/jarzebski/Arduino-BMP085
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <BMP085.h>

BMP085 bmp;

void setup() 
{
  Serial.begin(9600);

  // Initialize BMP085 or BMP180 sensor
  Serial.println("Initialize BMP085/BMP180");

  // Ultra high resolution: BMP085_OSS_ULTRA_HIGH_RES
  // (default) High resolution: BMP085_OSS_HIGH_RES
  // Standard: BMP085_OSS_STANDARD
  // Ultra low power: BMP085_OSS_ULTRA_LOW_POWER
  while(!bmp.begin())
  {
    Serial.println("Could not find a valid BMP085 or BMP180 sensor, check wiring!");
    delay(500);
  }

  // Check settings
  checkSettings();
}

void checkSettings()
{
  Serial.print("Oversampling: ");
  Serial.println(bmp.getOversampling());
}

void loop()
{
    Serial.print("rawTemp = ");
    Serial.print(bmp.readRawTemperature());
    Serial.print(" rawPressure = ");
    Serial.print(bmp.readRawPressure());
    Serial.println();

    delay(1000);
}
