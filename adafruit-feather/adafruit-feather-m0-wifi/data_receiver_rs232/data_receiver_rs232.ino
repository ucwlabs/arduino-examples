/* 
 * Example how to receive data via RS232
 * Copyright 2019 HackTheBase - UCW Labs Ltd. All rights reserved.
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