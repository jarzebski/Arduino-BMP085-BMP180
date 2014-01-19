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

  while(!bmp.begin())
  {
    Serial.println("Could not find a valid BMP085 or BMP180 sensor, check wiring!");
    delay(500);
  }
}

void loop()
{
    delay(1000);
    Serial.println("TODO");
}
