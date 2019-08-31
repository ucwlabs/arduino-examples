/* 
 * Example how to send data via RS232
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

void setup() {
  setupSerialPorts();
}

void loop() {
  String payload = "{\"msg\": \"Hello World!\"}";
  
  int bytesSent = sendData(payload);
  Serial.println("Payload " + payload + " has been sent. Size = " + bytesSent + " bytes");  

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

int sendData(String data) {
  if (data.length() < 1) {
    return 0;
  }

  /*
   * Serial.read has garbled data after a Serial.write
   * https://arduino.stackexchange.com/questions/910/serial-read-has-garbled-data-after-a-serial-write
   * 
   * ASCII Table and Description
   * http://www.asciitable.com
   * 
   * Sending Large Strings of Data to Arduino
   * https://ostanin.org/2012/01/08/sending-large-strings-of-data-to-arduino/
   */
  
  String dataBegin = String(char(2)); // Begin of message, symbol STX = 0x02, start of string
  String dataEnd = String(char(13)) + String(char(10)); // End of message, symbol ETX = 0x0D, 0x0A => \n, end of string

  while (Serial1.read() >= 0); // do nothing

  String payload = dataBegin + data + dataEnd;
  int bytesSent = Serial1.print(payload);
  
  Serial1.flush();

  return bytesSent;
}