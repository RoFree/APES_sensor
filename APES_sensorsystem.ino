#include "VernierLib.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define TEMP_PIN A1
#define SERIES_RES 10000

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

VernierLib co2sensor;
double co2reading;
double temp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  co2sensor.ID(17);
  printSensorInfo();

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  //delay(2000);
  display.clearDisplay();

  pinMode(TEMP_PIN, INPUT);

}

void loop() {
  co2reading = co2sensor.readSensor();
  temp = readTherm();
  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("CO2 reading = ");
  Serial.print(co2reading);
  Serial.print(" ");
  Serial.println(co2sensor.sensorUnits());

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.print("CO2 Reading"); 
  display.setCursor(0, 15);
  display.setTextSize(2);
  display.print(co2reading);
  display.print(" ");
  display.setTextSize(1);
  display.print(co2sensor.sensorUnits());
  display.setCursor(0, 35);
  // Display static text
  display.print("Temperature"); 
  display.setCursor(0, 50);
  display.setTextSize(2);
  display.print(temp);
  display.print(" ");
  display.setTextSize(1);
  display.print("\xB0");
  display.print("C");
  display.display();
  
  delay(200); 

}

double readTherm(){
  double reading = 0;
  for(int i=0; i<100; i++){
    reading += analogRead(TEMP_PIN);
  }
  reading /= 100.0;
  reading = SERIES_RES / (1024.0/reading -1.0);
  reading = log(reading / 10000) / 3950;
  reading += 1.0/(25+273.15);
  reading = 1.0/reading;
  reading -= 273.15;
  return reading;
  
}
 void printSensorInfo()
 {
   // print out information about the sensor found:
   Serial.println("Sensor Information:"); 
   Serial.print("Sensor ID number: "); 
   Serial.print("\t");
   Serial.println(co2sensor.sensorNumber());
   Serial.print("Sensor Name: ");
   Serial.print("\t");
   Serial.println(co2sensor.sensorName());
   Serial.print("Short Name: "); 
   Serial.print("\t");
   Serial.println(co2sensor.shortName()); 
   Serial.print("Units: "); 
   Serial.print("\t");
   Serial.println(co2sensor.sensorUnits());
   Serial.print("ID voltage level: "); 
   Serial.print("\t");
   Serial.println(co2sensor.voltageID());
   Serial.print("Page: ");  
   Serial.print("\t");
   Serial.println(co2sensor.page());
   Serial.print("slope: ");
   Serial.print("\t");
   Serial.println(co2sensor.slope());
   Serial.print("intercept: ");
   Serial.print("\t");
   Serial.println(co2sensor.intercept());
   Serial.print("cFactor:");
   Serial.print("\t");
   Serial.println(co2sensor.cFactor());
   Serial.print("calEquationType: ");
   Serial.print("\t");
   Serial.println(co2sensor.calEquationType());
 }
