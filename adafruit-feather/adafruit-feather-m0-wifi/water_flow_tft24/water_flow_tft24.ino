/*
 * Water Flow Measurement
 * Copyright 2019 HackTheBase - UCW Labs Ltd. All rights reserved.
 *
 * The example was created for the HackTheBase IoT Hub Lab.
 *
 * The HackTheBase IoT Hub Lab is a creative space dedicated to prototyping and inventing, 
 * all sorts of microcontrollers (Adafruit Feather, Particle Boron, ESP8266, ESP32 and more) 
 * with various types of connectivity (WiFi, LoRaWAN, GSM, LTE-M), and tools such as screwdriver,
 * voltmeter, wirings, as well as an excellent program filled with meetups, workshops, 
 * and hackathons to the community members. 
 *
 * https://hackthebase.com/iot-hub-lab
 * 
 * The HackTheBase IoT Hub will give you access to the infrastructure dedicated to your project,
 * and you will get access to our Slack community.
 *
 * https://hackthebase.com/iot-hub
 *
 * As a member, you will get access to the hardware and software resources 
 * that can help you to work on your IoT project.
 *
 * https://hackthebase.com/register
 *
 * 
 * Water Flow Sensor - 1/8" SKU: SEN0216
 * https://www.dfrobot.com/product-1531.html
 * https://www.dfrobot.com/wiki/index.php/Water_Flow_Sensor_-_1/8%22_SKU:_SEN0216#More
 * 
 * Reading liquid flow rate using an Arduino 
 * https://www.robotics.org.za/index.php?route=product/product&product_id=411 
 */

#include <SPI.h>
#include <WiFi101.h>
#include "Adafruit_GFX.h"
#include <Adafruit_ILI9341.h>

#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5

/*
 * Adafruit 2.4" TFT FeatherWing
 * https://learn.adafruit.com/adafruit-2-4-tft-touch-screen-featherwing?view=all
 */
 
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int hallsensor = A2; // The pin location of the sensor
volatile double waterFlow;

void setup() {
  Serial.begin(9600);        

  /*
   * Interrupts Feather M0 from Adafruit
   * https://github.com/arduino/ArduinoCore-samd/issues/136 
   */
   
  pinMode(hallsensor, INPUT);
  attachInterrupt(A2, pulse, RISING);

  Serial.println("Water Flow Measurement");

  waterFlow = 0;
  
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  tft.setCursor(0, 0);

  /*
   * Have print() clear existing text
   * https://github.com/adafruit/Adafruit-GFX-Library/issues/16
   */

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.print("waterFlow:");
  tft.print(waterFlow);
  tft.print("   L");

  Serial.print("waterFlow:");
  Serial.print(waterFlow);
  Serial.println("   L");
  
  delay(500);
}

void pulse() { //measure the quantity of square wave
  waterFlow += 1.0 / 5880.0;
}