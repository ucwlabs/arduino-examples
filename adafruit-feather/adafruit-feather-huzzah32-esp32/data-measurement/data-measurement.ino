/* 
 * Data measurement example for Adafruit HUZZAH32 - ESP32 Feather
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

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "your_ssid";     // your network SSID (name)
char pass[] = "your_password"; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;              // your network key Index number (needed only for WEP)
byte mac[6];
int status = WL_IDLE_STATUS;

unsigned long lastConnectionTime = 0;               // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 15L * 1000L;  // delay between updates, in milliseconds

#define UCW_API_HOST      "cloud.dev.unitycloudware.com"
#define UCW_API_PORT      80
#define UCW_DEVICE_TOKEN  "your_token"
#define UCW_DEVICE_ID     "your_deviceID"
#define UCW_DATA_STREAM   "data-monitoring"
#define UCW_CLIENT_NAME   "Adafruit-Feather-HUZZAH32-ESP32"

IPAddress server;
WiFiClient client;

void setup() {
  setupSerialPorts();
  setupWifi();
}

void loop() {
  collectData();
  delay(1000);
}

void setupSerialPorts() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void setupWifi() {
  /*
   * Adafruit HUZZAH32 - ESP32 Feather
   * https://learn.adafruit.com/adafruit-huzzah32-esp32-feather?view=all
   */

  WiFi.enableSTA(true);
  delay(2000);

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, pass);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.status();
    
    // wait 10 seconds for connection:
    delay(10000);
  }
  
  Serial.println("Connected to WiFi!");
  
  printWifiStatus();

  WiFi.hostByName(UCW_API_HOST, server);
}

void printWifiStatus() {
  Serial.println();
  
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  WiFi.macAddress(mac);
  Serial.print("MAC Address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
  
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.println();
}

void collectData() {
  if ((millis() - lastConnectionTime) > postingInterval) {
    String data = "";

    Serial.print("Data = ");

    data = readData();
    Serial.print(data);

    Serial.println("");

    if (data.length() > 0) {
      sendData(data);      
    }
  }
}

String readData() {
  double temperature = 22.00;
  int humidity = 43;
  
  String data = "{\"temperature\": %temperature, \"humidity\": %humidity}";
  data.replace("%temperature", String(temperature));
  data.replace("%humidity", String(humidity));
  
  return data;
}

void sendData(String payload) {
  if (payload.length() < 1) {
    Serial.println("No data to send!");
    return;
  }

  if (client.connect(server, UCW_API_PORT)) {
    Serial.println();
    Serial.println("Request:");
    Serial.println();
    Serial.println("Sending payload: " + payload);
    Serial.print("Payload length: ");
    Serial.println(payload.length());

    String apiUri = "POST /api/ucw/v1/data-streams/%dataStreamName/messages/%deviceID HTTP/1.1";
    apiUri.replace("%deviceID", UCW_DEVICE_ID);
    apiUri.replace("%dataStreamName", UCW_DATA_STREAM);

    Serial.print("API URI: ");
    Serial.println(apiUri);
    
    client.println(apiUri);
    client.print("Host: ");
    client.println(UCW_API_HOST);
    client.print("User-Agent: ");
    client.println(UCW_CLIENT_NAME);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.print("Authorization: Bearer ");
    client.println(UCW_DEVICE_TOKEN);
    client.println();
    client.println(payload);

    Serial.println();
    Serial.println("Response:");
    Serial.println();

    while (client.connected()) {
      while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
    }

    // note the time that the connection was made:
    lastConnectionTime = millis();

    // close any connection before send a new request.
    // This will free the socket on the WiFi shield
    client.stop();

  } else {
    // if you couldn't make a connection
    Serial.println("connection failed");
  }
}