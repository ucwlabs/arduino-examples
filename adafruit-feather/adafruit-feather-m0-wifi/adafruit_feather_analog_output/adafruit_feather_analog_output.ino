/* 
 * Analog Output
 * Copyright 2019 HackTheBase - UCW Labs Ltd. All rights reserved.
 */

const int analogOutPin = A0; // V(out)

void setup() { 
  setupSerialPorts();

  /*
   * Feather M0 Analog Input Resolution
   * https://forums.adafruit.com/viewtopic.php?f=57&t=103719&p=519042
   */
  analogReadResolution(12); // Analog Input Resolution
}

void setupSerialPorts() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void doWaves() {
  Serial.println("Generating waves...");
  
  for (int i = 0; i <= 1023; i++) {
    Serial.print("DAC = ");
    Serial.println(i);
    analogWrite(analogOutPin, i); // Up ramp to DAC A0
    delay(30);
  }

  delay(1000);

  for (int i = 1023; i > 0; i--) {
    Serial.print("DAC = ");
    Serial.println(i);
    analogWrite(A0, i); // Down ramp to DAC A0
    delay(30);
  }

  delay(1000);
}

void loop() {
  
  /*
   * Feather M0 DAC output range
   * https://forums.adafruit.com/viewtopic.php?f=57&t=94524
   * 
   * Feather M0 DAC not outputting 10 bit
   * https://forums.adafruit.com/viewtopic.php?f=57&t=143256&p=707076&hilit=feather+m0+analog+output
   */
   
  int voltageOut = 1023; // Range 0 - 1023 => 0 - 3.3V
  Serial.print("voltageOut = ");
  Serial.println(voltageOut);
  analogWrite(analogOutPin, voltageOut);
  delay(30);

  //doWaves();
}