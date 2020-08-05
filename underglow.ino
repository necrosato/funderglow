//
// underglow.ino
// Naookie Sato
//
// Created by Naookie Sato on 08/01/2020
// Copyright © 2020 Naookie Sato. All rights reserved.
//

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "RgbLed.h"
#include "SetHtml.h"

const char AP_NAME[] = "underglow";
const char WiFiAPPSK[] = "underglow";

int wifiStatus;
IPAddress ip(172,16,168,1);      // this node's soft ap ip address
IPAddress gateway(172,16,168,1); // this node's soft ap gateway
IPAddress subnet(255,255,255,0); // this node's soft ap subnet mask
ESP8266WebServer server(80);

std::vector<RgbLed> leds;

void setupWiFi()
{
  Serial.print("This device's MAC address is: ");
  Serial.println(WiFi.softAPmacAddress());

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(AP_NAME, WiFiAPPSK, 6, 0);
  Serial.print("This AP's IP address is: ");
  Serial.println(WiFi.softAPIP());  
}

void initHardware()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW);//on Lolin ESP8266 v3 dev boards, the led is active low
  delay(1000);
}

void handleSetColors()
{
  String message = "Setting colors:\n";
  if (server.hasArg("red"))
  {
    for (auto led : leds)
    {
      message += "    red: " + server.arg("red") + "\n";
      int rval = server.arg("red").toInt();
      led.red(rval);
    }
  }
  if (server.hasArg("blue"))
  {
    for (auto led : leds)
    {
      message += "    blue: " + server.arg("blue") + "\n";
      int bval = server.arg("blue").toInt();
      led.blue(bval);
    }
  }
  if (server.hasArg("green"))
  {
    for (auto led : leds)
    {
      message += "    green: " + server.arg("green") + "\n";
      int gval = server.arg("green").toInt();
      led.green(gval);
    }
  }

  //server.send(200, "text/plain", message);
  server.send(200, "text/html", setHtml);
  Serial.println(message);
}

void handleTestLeds()
{
  for (auto led : leds)
  {
    led.off();
    delay(1000);
    for (auto pin : led.pins())
    {
      for( int i = 255; i > 0; --i)
      {
        analogWrite(pin, i);     
        delay(10);
      } 
      delay(500);
    }
    delay(1000);
    led.on();
    delay(1000);
  }
}

void setup()
{
  initHardware();
  setupWiFi();
  leds.emplace_back(D5, D6, D7);
  server.on("/set", handleSetColors);
  server.on("/test", handleTestLeds);
  server.begin();
}

void loop()
{
  server.handleClient();
  delay(100);
}
