/*
 Web Dimmer
 
 An application to dim a light through the web.

 The Python code for the web server is available at:
 https://github.com/circuitar/WebLights
 
 Circuit:
 * Base Board or Base Board UNO
 * Alevino + USB Nanoshield or Arduino UNO
 * Ethernet Nanoshield
 * Triac Nanoshield
 * Zero Cross Nanoshield

 Copyright (c) 2013 Circuitar
 This software is released under the MIT license. See the attached LICENSE file for details.
 */

#include <SPI.h>
#include <Wire.h>
#include <Ethernet.h>
#include "Nanoshield_EEPROM.h"
#include "TimerOne.h"

#define DEVICE "light1"
#define HTTP_TIMEOUT 3000
#define HTTP_RECV_BUF_SIZE 300
#define MAX_TRIAC_DELAY 7331
#define REQUEST_DELAY 200

int triac = 3; // Triac: pin D3
int zc = 2;    // Detetor de zero: pin D2

// Time between zero detection and triac activation
int time;

// Peripherals
Nanoshield_EEPROM eeprom(1, 1, 0, true);
EthernetClient client;

// Local MAC address
byte mac[6];

// Server IP address
//byte serverIp[] = { 192, 168, 2, 1 }; // Ethernet point-to-point
byte serverIp[] = { 192, 168, 1, 102 }; // Local network
int serverPort = 8001;

// Local IP address
boolean dhcp = true;
//byte localIp[] = { 192, 168, 2, 2 }; // Ethernet point-to-point
byte localIp[] = { 192, 168, 1, 222 }; // Local network

// HTTP receive buffer
char httpRxBuf[HTTP_RECV_BUF_SIZE];

void setup()
{
  // Initialize triac and zero cross pins
  pinMode(triac, OUTPUT);
  pinMode(zc, INPUT);
  
  Serial.begin(115200);
  while (!Serial) {}
  
  // Read MAC address from EEPROM
  eeprom.begin();
  eeprom.startReading(0x00FA);
  for (int i = 0; i < 6; i++) {
    mac[i] = eeprom.read();
  }

  // Initialize Ethernet
  if (dhcp) {
    Serial.print(F("Attempting DHCP... "));
    if (!Ethernet.begin(mac)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK"));
    }
  } else {
    Ethernet.begin(mac, localIp);
  }
  
  // Print connection data
  Serial.print(F("MAC: "));
  Serial.print(mac[0], HEX);
  for (int i = 1; i < 6; i++) { 
    Serial.print(":");
    Serial.print(mac[i], HEX);
  }
  Serial.println();
  Serial.print(F("IP: "));
  Serial.println(Ethernet.localIP());

  // Turn off triac
  time = MAX_TRIAC_DELAY;

  // Associate the zero cross detector rising edge to
  //   the dimmer() function which activates the triac
  attachInterrupt(0, dimmer1, RISING);
  
  // Initialize timer 1
  Timer1.initialize(1000000);
}

void loop()
{
  // Send data to web server
  if(client.connect(serverIp, serverPort)) {
    // Send HTTP GET
    client.println(F("GET /get/" DEVICE "/ HTTP/1.1"));
    client.print(F("Host: "));
    client.print(serverIp[0]);
    client.print(".");
    client.print(serverIp[1]);
    client.print(".");
    client.print(serverIp[2]);
    client.print(".");
    client.print(serverIp[3]);
    client.print(":");
    client.println(serverPort);
    client.println("User-Agent: Arduino/1.0");
    client.println(F("Connection: close"));
    client.println();

    // Read device value from server, checking for timeout
    unsigned long t0 = millis();
    int count = 0;
    boolean header = true;
    httpRxBuf[0] = '\0';
    while (client.connected() && millis() - t0 < HTTP_TIMEOUT && count < HTTP_RECV_BUF_SIZE - 1) {
      if (client.available()) {
        httpRxBuf[count++] = client.read();
        if (header) {
          // Wait for "\r\n\r\n" (end of header)
          if (count > 4 &&
            httpRxBuf[count - 4] == '\r' && httpRxBuf[count - 3] == '\n' && 
            httpRxBuf[count - 2] == '\r' && httpRxBuf[count - 1] == '\n'
          ) {
            // Discard the header
            count = 0;
            header = false;
          }
        }
      }
    }
    httpRxBuf[count] = '\0';
    client.stop();
  } else {
    Serial.println(F("Unable to connect to server"));
  }
  
  // TODO: validate input, setting "t" to MAX_TRIAC_DELAY if invalid
  int t = MAX_TRIAC_DELAY - int(atof(httpRxBuf) * (MAX_TRIAC_DELAY - 1) / 100.0);

  // Update the "time" variable, disabling interrupts so there is no
  //   conflict accessing it.
  noInterrupts();
  time = t;
  interrupts();
  
  // Wait for some time between requests
  delay(REQUEST_DELAY);
}

void dimmer1()
{
  // Deactivate the triac
  digitalWrite(triac, LOW);

  // Wait until the right time to activate the triac (based on the "time" variable)
  Timer1.start();
  Timer1.attachInterrupt(dimmer2, time + 500);
}

void dimmer2()
{
  // Ignore initial interrupr
  Timer1.attachInterrupt(dimmer3);
}

void dimmer3()
{
  // Activate triac
  digitalWrite(triac, HIGH);
  Timer1.stop();
}
