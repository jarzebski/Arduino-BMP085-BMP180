/*
  BMP085 / BMP180 Barometric Pressure & Temperature Sensor. Simple Example (Floating-point equations)
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/czujniki-cisnienia-bmp085-bmp180.html
  GIT: https://github.com/jarzebski/Arduino-BMP085-BMP180
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski

  From Weather Station Data Logger project
  "Integer math results in stair-step, jumpy corrections as the input values
  vary smoothly. This is due to round-off errors in the integer calculations.
  The floating point math does not suffer from this problem and corrections
  vary smoothly with changes in input values."
  http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf
*/

#include <Wire.h>
#include <BMP085.h>

BMP085 bmp;

void setup() 
{
  Serial.begin(9600);

  // Initialize BMP085 or BMP180 sensor
  Serial.println("Initialize BMP085/BMP180 Serial");

  // Ultra high resolution: BMP085_ULTRA_HIGH_RES
  // (default) High resolution: BMP085_HIGH_RES
  // Standard: BMP085_STANDARD
  // Ultra low power: BMP085_ULTRA_LOW_POWER
  while(!bmp.begin(BMP085_ULTRA_HIGH_RES))
  {
    Serial.println("Could not find a valid BMP085 or BMP180 sensor, check wiring!");
    delay(500);
  }

  // Enable or disable SOSS (Software oversampling)- Use with BMP085_ULTRA_HIGH_RES !
  // For applications where a low noise level is critical, averaging is recommended if the lower bandwidth is acceptable
  // Conversion time pressure: 76.5ms, RMS noise 0.02 hPA / 0.17 m
  bmp.setSoftwareOversampling(1);

  // Check settings
  checkSettings();
}

void checkSettings()
{
  long bmpVersion = bmp.getVersion();
  Serial.print("Chip version: ");
  Serial.print(bmpVersion >> 8); Serial.print("."); Serial.print(bmpVersion & 0xFF);
  Serial.print(" (0x"); Serial.print(bmpVersion, HEX); Serial.println(")");

  Serial.print("Oversampling: ");
  Serial.println(bmp.getOversampling());
  Serial.print("Software Oversampling: ");
  Serial.println(bmp.getSoftwareOversampling());
}

void loop()
{
  // Set you real altitude
  // My location: Poland, Bytom, 8 floor
  double myRealAltitude = 335;

  // Read true temperature & Pressure
  double realTemperature = bmp.readFloatTemperature();
  double realPressure = bmp.readFloatPressure();

  // Calculate sealevel pressure
  double seaLevelPressure = bmp.getSeaLevel(realPressure, myRealAltitude);

  Serial.println("--");

  Serial.print("realTemp = ");
  Serial.print(realTemperature);
  Serial.print(" *C");

  Serial.print(", realPressure = ");
  Serial.print(realPressure/100);
  Serial.print(" hPa");

  Serial.print(" seaLevelPressure = ");
  Serial.print(seaLevelPressure/100);
  Serial.println(" hPa");

  delay(1000);
}
