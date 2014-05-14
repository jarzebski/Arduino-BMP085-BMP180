/*
  BMP085 / BMP180 Barometric Pressure & Temperature Sensor. Output for BMP085_processing.pde
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/czujniki-cisnienia-bmp085-bmp180.html
  GIT: https://github.com/jarzebski/Arduino-BMP085-BMP180
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
 */

#include <Wire.h>
#include <BMP085.h>

BMP085 bmp;

void setup() 
{
  Serial.begin(115200);

  // Initialize BMP085 or BMP180 sensor
  // Ultra high resolution: BMP085_ULTRA_HIGH_RES
  // (default) High resolution: BMP085_HIGH_RES
  // Standard: BMP085_STANDARD
  // Ultra low power: BMP085_ULTRA_LOW_POWER
  while(!bmp.begin(BMP085_ULTRA_HIGH_RES))
  {
    delay(500);
  }
}

void loop()
{
  // Disable Software Oversampling
  bmp.setSoftwareOversampling(0);
  
  // Read true temperature & Pressure (Integer equations)
  double realTemperature = bmp.readTemperature();
  long realPressure = bmp.readPressure();
  double realAltitude = bmp.getAltitude(realPressure);

  // Read true temperature & Pressure (Floating-point equations)
  double floatTemperature = bmp.readFloatTemperature();
  double floatPressure = bmp.readFloatPressure();
  double floatAltitude = bmp.getAltitude(floatPressure);

  // Enable Software Oversampling

  // Read true temperature & Pressure (Floating-point equations) /.Software oversampling
  bmp.setSoftwareOversampling(1);
  double sossTemperature = bmp.readFloatTemperature();
  double sossPressure = bmp.readFloatPressure();
  double sossAltitude = bmp.getAltitude(sossPressure);
  
  // Output
  Serial.print(realTemperature);
  Serial.print(":");
  Serial.print(floatTemperature);
  Serial.print(":");
  Serial.print(sossTemperature);
  Serial.print(":");
  Serial.print(realPressure);
  Serial.print(":");
  Serial.print(floatPressure);
  Serial.print(":");
  Serial.print(sossPressure);
  Serial.print(":");
  Serial.print(realAltitude);
  Serial.print(":");
  Serial.print(floatAltitude);    
  Serial.print(":");
  Serial.print(sossAltitude); 
  Serial.println();

}

