#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>
#include "colors.h"

#define PIN1 4
#define PIN2 5
#define PIN3 6
#define LED_COUNT 9
#define LED_COUNT3 9

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds2 = Adafruit_NeoPixel(LED_COUNT, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds3 = Adafruit_NeoPixel(LED_COUNT3, PIN3, NEO_GRB + NEO_KHZ800);

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);


double xaxis[10];
double yaxis[10];
double zaxis[10];

void displaySensorDetails(void) {
    sensor_t sensor;
    accel.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       ");
    Serial.println(sensor.name);
    Serial.print("Driver Ver:   ");
    Serial.println(sensor.version);
    Serial.print("Unique ID:    ");
    Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    ");
    Serial.print(sensor.max_value);
    Serial.println(" m/s^2");
    Serial.print("Min Value:    ");
    Serial.print(sensor.min_value);
    Serial.println(" m/s^2");
    Serial.print("Resolution:   ");
    Serial.print(sensor.resolution);
    Serial.println(" m/s^2");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

void setup(void) {
#ifndef ESP8266
    while (!Serial);
#endif
    Serial.begin(9600);
    Serial.println("Accelerometer Test");
    Serial.println("");
    if (!accel.begin()) {
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        while (1);
    }
    displaySensorDetails();
    leds.begin(); 
    leds2.begin();
    leds3.begin();
    clearLEDs(); 
}

void loop(void) {
    sensors_event_t event;
    accel.getEvent(&event);

    int X = event.acceleration.x;
    int Y = event.acceleration.y;
    int Z = event.acceleration.z;
    Serial.print("X: ");
    Serial.print(X);
    Serial.print("  ");
    Serial.print("Y: ");
    Serial.print(Y);
    Serial.print("  ");
    Serial.print("Z: ");
    Serial.print(Z);
    Serial.print("  ");
    Serial.println("m/s^2 ");
    updateArrays(X, Y, Z);
//    printArrays();
//    rainbow(RED);
//    rainbow2(GREEN);
//    rainbow3(BLUE);
    clearLEDs();
    if (changeDetected())
    {
        if(fromTo())
        {
           rainbow2(GREEN);
        }
    }
    else {
        if(X<=-7)
        {
            rainbow3(RED);
        }
        else if(Y >= 7 && Z > -7)
        {
            rainbow(RED);
        }
        else if (Z <= -7 && Y < 7)
        {
            rainbow2(RED);
        }
        else if (Z == -7 && Y == 7)
        {
            rainbow(RED);
            rainbow2(RED);
        }
        else if (X <= 1 && Y <= -2 && Z <= -2)
        {
            rainbow2(GREEN);
        }
    }
//      rainbow(AQUAMARINE);
      
    delay(500);
}

bool fromTo()
{
    int i;
    bool flg1 = false;
    bool flg2 = false;
    bool flg3 = false;
    bool flg4 = false;
    bool flg5 = false;
    bool flg6 = false;
    
    for(i = 6; i < 10; i++)
    {
        if(yaxis[i] >= 7)
        {
            flg1 = true;
        }
        if(xaxis[i] <= -2)
        {
            flg2 = true;
        }
        if(yaxis[i] <= -2)
        {
            flg3 = true;
        }
        if(zaxis[i] <= -2)
        {
            flg4 = true;
        }
    }
    if(flg1 && flg2 && flg3 && flg4)
    {
        return true;
    }
    return false;
}
bool changeDetected()
{
  int i;
  int xmax = -100;
  int xmin = 100;
  int ymax = -100;
  int ymin = 100;
  int zmax = -100;
  int zmin = 100;
  
    for(i = 6; i < 10; i++)
    { 
        if(xaxis[i] > xmax)
        {
            xmax = xaxis[i];
        }
        if(xaxis[i] < xmin)
        {
            xmin = xaxis[i];
        }
        if(yaxis[i] > ymax)
        {
            ymax = yaxis[i];
        }
        if(yaxis[i] < ymin)
        {
            ymin = yaxis[i];
        }
        if(zaxis[i] > zmax)
        {
            zmax = zaxis[i];
        }
        if(zaxis[i] < zmin)
        {
            zmin = zaxis[i];
        }
    }
    if(xmax > xmin + 7)
    {
        return true;
    }
    if(ymax > ymin + 7)
    {
        return true;
    }
    if(ymax > ymin + 7)
    {
        return true;
    }
    return false;
}
void printArrays()
{
    int i;
    for(i = 0; i < 10; i++)
    { 
        Serial.print(xaxis[i]);
        Serial.print(", ");
    }
    Serial.println();
    for(i = 0; i < 10; i++)
    { 
        Serial.print(yaxis[i]);
        Serial.print(", ");
    }
    Serial.println();
    for(i = 0; i < 10; i++)
    { 
        Serial.print(zaxis[i]);
        Serial.print(", ");
    }
    Serial.println();
}

void updateArrays(int x, int y, int z)
{
  int i;
  for(i = 1; i < 10; i++)
  {
      xaxis[i-1] = xaxis[i];
      yaxis[i-1] = yaxis[i];
      zaxis[i-1] = zaxis[i];
  }
  xaxis[9] = x;
  yaxis[9] = y;
  zaxis[9] = z;
}

void clearLEDs() {
    for (int i = 0; i < LED_COUNT3; i++) {
        leds.setPixelColor(i, 0);
        leds2.setPixelColor(i, 0);
        leds3.setPixelColor(i, 0);
    }
    leds.show();
      leds2.show();
      leds3.show();
}
void blnk(unsigned long color) {
    for (int i = 0; i < LED_COUNT; i++) {
        leds.setPixelColor(i, color);
    }
    leds.show();
    delay(500);
    for (int i = 0; i < LED_COUNT; i++) {
      leds.setPixelColor(i,0);
    }
    leds.show();
    delay(500);

}
void blnk2(unsigned long color) {
    for (int i = 0; i < LED_COUNT; i++) {
        leds2.setPixelColor(i, color);
    }
    leds2.show();
    delay(500);
    for (int i = 0; i < LED_COUNT; i++) {
      leds2.setPixelColor(i,0);
    }
    leds2.show();
    delay(500);

}
void rainbow(unsigned long color) {
    for (int i = 0; i < LED_COUNT; i++) {
        leds.setPixelColor(i, color);
    }
    leds.show();
}

void rainbow3(unsigned long color) {
    for (int i = 0; i < LED_COUNT3; i++) {
        leds3.setPixelColor(i, color);
    }
    leds3.show();
}

void rainbow2(unsigned long color) {
    for (int i = 0; i < LED_COUNT; i++) {
        leds2.setPixelColor(i, color);
    }
    leds2.show();
}


