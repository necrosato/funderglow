//
// funderglow.ino
// Naookie Sato
//
// Created by Naookie Sato on 08/01/2020
// Copyright © 2020 Naookie Sato. All rights reserved.
//

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "RgbLed.h"
#include "SetHtml.h"

const char AP_NAME[] = "funderglow";
const char WiFiAPPSK[] = "funderglow";

int wifiStatus;
IPAddress ip(172,16,168,1);      // this node's soft ap ip address
IPAddress gateway(172,16,168,1); // this node's soft ap gateway
IPAddress subnet(255,255,255,0); // this node's soft ap subnet mask
ESP8266WebServer server(80);

RgbLed led(D5, D6, D7);

std::function<void()> handleClient = []() {
  MDNS.update();
  server.handleClient();
};

std::function<void()> delay100 = []() { delay(100); };
std::function<void()> loopFunc = delay100;

void setupWiFi()
{
  Serial.print("This device's MAC address is: ");
  Serial.println(WiFi.softAPmacAddress());

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(AP_NAME, WiFiAPPSK, 6, 0);
  Serial.print("This AP's IP address is: ");
  Serial.println(WiFi.softAPIP());  
  if (MDNS.begin("funderglow"))
  {
    Serial.println("MDNS funderglow started");
  }
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
  loopFunc = delay100;
  String message = "Setting colors:\n";
  if (server.hasArg("red"))
  {
    message += "    red: " + server.arg("red") + "\n";
    int rval = server.arg("red").toInt();
    led.red(rval);
  }
  if (server.hasArg("blue"))
  {
    message += "    blue: " + server.arg("blue") + "\n";
    int bval = server.arg("blue").toInt();
    led.blue(bval);
  }
  if (server.hasArg("green"))
  {
    message += "    green: " + server.arg("green") + "\n";
    int gval = server.arg("green").toInt();
    led.green(gval);
  }

  server.send(200, "text/html", setHtml);
  Serial.println(message);
}

void testLeds()
{
  led.off();
  for (auto pin : led.pins())
  {
    led.ramp(pin, PWMRANGE, 0, 2, handleClient);
    delay(100);
  }
  led.ramp(led.pins(), 0, PWMRANGE, 2, handleClient);
  delay(1000);
  led.off();
}

void waveLeds()
{
  led.red(PWMRANGE);
  led.green(0);
  led.blue(0);
  delay(500);
  int freq = 3;
  led.ramp(led.gPin(), 0, PWMRANGE, freq, handleClient);
  led.ramp(led.rPin(), PWMRANGE, 0, freq, handleClient);
  led.ramp(led.bPin(), 0, PWMRANGE, freq, handleClient);
  led.ramp(led.gPin(), PWMRANGE, 0, freq, handleClient);
  led.ramp(led.rPin(), 0, PWMRANGE, freq, handleClient);
  led.ramp(led.gPin(), 0, PWMRANGE, freq, handleClient);
  led.ramp(led.bPin(), PWMRANGE, 0, freq, handleClient);
  led.ramp(led.gPin(), PWMRANGE, 0, freq, handleClient);
}

void smokeLeds()
{
  led.green(0);
  led.red(PWMRANGE);
  led.blue(0);
  delay(750);
  led.green(0);
  led.red(0);
  led.blue(PWMRANGE);
  delay(750);
}

std::function<void()> getHandler(std::function<void()> f, String msg)
{
  return [f, msg]()
  {
    loopFunc = f;
    server.send(200, "text/html", setHtml);
    Serial.println(msg);
  };
}

void setup()
{
  initHardware();
  setupWiFi();
  server.on("/", handleSetColors);
  server.on("/set", handleSetColors);
  server.on("/test", getHandler(&testLeds, "test"));
  server.on("/wave", getHandler(&waveLeds, "wave"));
  server.on("/smoke", getHandler(&smokeLeds, "smoke"));
  server.begin();
}

void loop()
{
  handleClient();
  delay(100);
  loopFunc();
}
