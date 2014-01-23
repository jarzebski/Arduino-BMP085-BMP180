/*
    BMP085/BMP180 Triple Axis Accelerometer. Processing for BMP085_processing.ino
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-akcelerometr-BMP085/BMP180.html
    GIT: https://github.com/jarzebski/Arduino-BMP085-BMP180
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

import processing.serial.*;

Serial myPort;

// Data samples
int actualSample = 0;
int maxSamples = 70;
int sampleStep = 10;
boolean hasData = false;

// Charts
PGraphics pgChart;
int[] colors = { #ff4444, #33ff99, #5588ff };
String[] compareSeries = { "Integer", "Float", "Float+SOSS" };

// Data for compare
float[][] pressureValues = new float[3][maxSamples];
float[][] temperatureValues = new float[3][maxSamples];
float[][] altitudeValues = new float[3][maxSamples];

void setup ()
{
  size(770, 670, P2D);
  background(0);

  // Serial
  myPort = new Serial(this, Serial.list()[0], 115200);
  myPort.bufferUntil(10);
}

void drawChart(String title, String[] series, float[][] chart, int x, int y, int h, int min, int max, int step) 
{
  int actualColor = 0;

  pgChart = createGraphics((maxSamples*sampleStep)+50, h+60);

  pgChart.beginDraw();

  // Draw chart area and title
  pgChart.background(0);
  pgChart.strokeWeight(1);
  pgChart.noFill();
  pgChart.stroke(50);
  pgChart.rect(0, 0, (maxSamples*sampleStep)+49, h+59);
  pgChart.text(title, ((maxSamples*sampleStep)/2)-(textWidth(title)/2)+40, 20);

  // Draw chart description
  String Description[] = new String[chart.length];
  int DescriptionWidth[] = new int[chart.length];
  int DesctiptionTotalWidth = 0;
  int DescriptionOffset = 0;

  for (int j = 0; j < chart.length; j++)
  {
     Description[j] = "  "+series[j]+" = ";
     DescriptionWidth[j] += textWidth(Description[j]+"+000000.00");
     Description[j] += nf(chart[j][actualSample-1], 0, 2)+"  ";
     DesctiptionTotalWidth += DescriptionWidth[j];
  }

  actualColor = 0;

  for (int j = 0; j < chart.length; j++)
  {
    pgChart.fill(colors[actualColor]);
    pgChart.text(Description[j], ((maxSamples*sampleStep)/2)-(DesctiptionTotalWidth/2)+DescriptionOffset+40, h+50);
    DescriptionOffset += DescriptionWidth[j];
    actualColor++;
    if (actualColor >= colors.length) actualColor = 0;
  }

  // Draw H-Lines 
  pgChart.stroke(100);

  for (float t = min; t <= max; t=t+step)
  {
    float line = map(t, min, max, 0, h);
    pgChart.line(40, h-line+30, (maxSamples*sampleStep)+40, h-line+30);
    pgChart.fill(200, 200, 200);
    pgChart.textSize(12);
    pgChart.text(int(t), 5, h-line+34);
  }

  // Draw data series
  pgChart.strokeWeight(2);

  for (int i = 1; i < actualSample; i++)
  {
    actualColor = 0;

    for (int j = 0; j < chart.length; j++)
    {
      pgChart.stroke(colors[actualColor]);

      float d0 = chart[j][i-1];
      float d1 = chart[j][i];

      if (d0 < min) d0 = min;
      if (d0 > max) d0 = max;
      if (d1 < min) d1 = min;
      if (d1 > max) d1 = max;

      float v0 = map(d0, min, max, 0, h);
      float v1 = map(d1,   min, max, 0, h);

      pgChart.line(((i-1)*sampleStep)+40, h-v0+30, (i*sampleStep)+40, h-v1+30);

      actualColor++;

      if (actualColor >= colors.length) actualColor = 0;
    }
  }

  pgChart.endDraw();

  image(pgChart, x, y);
}

void draw() 
{
  if (!hasData) return;

  background(0);

  drawChart("Pressure compare", compareSeries, pressureValues, 10, 10, 150, (int)getMin(pressureValues),  (int)getMax(pressureValues), 10);
  drawChart("Temperature compare", compareSeries, temperatureValues, 10, 230, 150, (int)getMin(temperatureValues),  (int)getMax(temperatureValues), 1);
  drawChart("Altitude compare", compareSeries, altitudeValues, 10, 450, 150, (int)getMin(altitudeValues),  (int)getMax(altitudeValues), 1);
}

float getMin(float[][] chart)
{
  float minValue = 0;
  float[] testValues = new float[3];
  float testMin = 0;

  for (int i = 0; i < actualSample; i++)
  {
    testValues[0] = chart[0][i];
    testValues[1] = chart[1][i];
    testValues[2] = chart[2][i];
    testMin = min(testValues);
    if (i == 0)
    {
      minValue = testMin;
    } else
    {
      if (minValue > testMin) minValue = testMin;
    }
  }
 
  return ceil(minValue)-1; 
}

float getMax(float[][] chart)
{
  float maxValue = 0;
  float[] testValues = new float[3];
  float testMax = 0;

  for (int i = 0; i < actualSample; i++)
  {
    testValues[0] = chart[0][i];
    testValues[1] = chart[1][i];
    testValues[2] = chart[2][i];
    testMax = max(testValues);
    if (i == 0)
    {
      maxValue = testMax;
    } else
    {
      if (maxValue < testMax) maxValue = testMax;
    }
  }
 
  return ceil(maxValue); 
}

void nextSample(float[][] chart)
{
    for (int j = 0; j < chart.length; j++)
    {
      float last = chart[j][maxSamples-1];

      for (int i = 1; i < maxSamples; i++)
      {
        chart[j][i-1] = chart[j][i];
      }

      chart[j][(maxSamples-1)] = last;
    }
}

void serialEvent (Serial myPort)
{
  String inString = myPort.readStringUntil(10);

  if (inString != null)
  {
    inString = trim(inString);
    String[] list = split(inString, ':');
    String testString = trim(list[0]);

    if (list.length != 9) return;

    // Temperature compare
    temperatureValues[0][actualSample] = (float(list[0]));
    temperatureValues[1][actualSample] = (float(list[1]));
    temperatureValues[2][actualSample] = (float(list[2]));

    // Pressure compare
    pressureValues[0][actualSample] = (float(list[3]));
    pressureValues[1][actualSample] = (float(list[4]));
    pressureValues[2][actualSample] = (float(list[5]));

    // Pressure compare
    altitudeValues[0][actualSample] = (float(list[6]));
    altitudeValues[1][actualSample] = (float(list[7]));
    altitudeValues[2][actualSample] = (float(list[8]));
    
    if (actualSample > 1)
    {
      hasData = true;
    }

    if (actualSample == (maxSamples-1))
    {
      nextSample(temperatureValues);
      nextSample(pressureValues);
      nextSample(altitudeValues);
    } else
    {
      actualSample++;
    }
  }
}
