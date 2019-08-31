/* 
 * Example how to receive data via RS232
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
 */

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 5L * 1000L; // delay between updates, in milliseconds

void setup() {
  setupSerialPorts();
}

void loop() {
  String data = "";

  if (Serial1.available()) {
    data = readSerial();
  }

  if ((millis() - lastConnectionTime > postingInterval) && data.length() > 0) {
    Serial.println("Received payload: " + data + ", Size = " + data.length() + " bytes");
    lastConnectionTime = millis();
  }

  delay(1000);
}

void setupSerialPorts() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

String readSerial() {
  String input = "";

  boolean processing = false;

  while (Serial1.available() > 0) {
    int incomingByte = Serial1.read();
    delay(1);

    if (incomingByte == '') { // Begin of message, symbol STX = 0x02, start of string
      input = "";
      processing = true;
    }

    else if (incomingByte == '\n') { // End of message, symbol ETX = 0x0D, 0x0A => \n, end of string
      processing = false;
    }
    
    else if (processing) {
      input.concat((char) incomingByte);
    }
  }

  return input;
}