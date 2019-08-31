/* 
 * Example how to send data via RS232
 * Copyright 2019 HackTheBase - UCW Labs Ltd. All rights reserved.
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