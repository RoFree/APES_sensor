#include "VernierLib.h"
#include <ESP8266WiFi.h>
#include <DNSServer.h> 
#include <ESP8266WebServer.h>

VernierLib co2sensor;
double co2reading;

const char* SSID_NAME = "APES Aquarium";

// Init system settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  co2sensor.ID(17);
  printSensorInfo();

  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID_NAME); 

  // Start webserver
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only for HTTP)
  webServer.on("/",[]() { webServer.send(HTTP_CODE, "text/html", page());});
  webServer.onNotFound([]() { webServer.send(HTTP_CODE, "text/html", "Not found"); });
  webServer.begin()


}

void loop() {
  co2reading = co2sensor.readSensor();
  Serial.print("CO2 reading = ");
  Serial.print(co2reading);
  Serial.print(" ");
  Serial.println(co2sensor.sensorUnits());
  
  dnsServer.processNextRequest(); webServer.handleClient();
  delay(200); 

}

String page(){
  return co2reading;
}

 void printSensorInfo()
 {
   // print out information about the sensor found:
   Serial.println("Sensor Information:"); 
   Serial.print("Sensor ID number: "); 
   Serial.print("\t");
   Serial.println(Vernier.sensorNumber());
   Serial.print("Sensor Name: ");
   Serial.print("\t");
   Serial.println(Vernier.sensorName());
   Serial.print("Short Name: "); 
   Serial.print("\t");
   Serial.println(Vernier.shortName()); 
   Serial.print("Units: "); 
   Serial.print("\t");
   Serial.println(Vernier.sensorUnits());
   Serial.print("ID voltage level: "); 
   Serial.print("\t");
   Serial.println(Vernier.voltageID());
   Serial.print("Page: ");  
   Serial.print("\t");
   Serial.println(Vernier.page());
   Serial.print("slope: ");
   Serial.print("\t");
   Serial.println(Vernier.slope());
   Serial.print("intercept: ");
   Serial.print("\t");
   Serial.println(Vernier.intercept());
   Serial.print("cFactor:");
   Serial.print("\t");
   Serial.println(Vernier.cFactor());
   Serial.print("calEquationType: ");
   Serial.print("\t");
   Serial.println(Vernier.calEquationType());
 }
