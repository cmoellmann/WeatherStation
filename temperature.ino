/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "OneWire.h"
#include "DallasTemperature.h"
//#include "Stocks.h"
#include "WebData.h"
#include "Display.h"

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

#define MAX_STR_LEN 12
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

bool firstExec = true;
float tOut = 0.0;
float tIn = 0.0;
String wind;
String humidity;
float stockValue = 0.0;
float stockChange = 0.0;

TCPClient client;
Adafruit_SSD1306 display(OLED_RESET);
Display myDisplay(&display);
enum displayType_t displayType = MULTI;

#define NUM_STOCKS 3
stock_t stocks[NUM_STOCKS] = {
  { "Global Sel Div 100", "globselect.php", 27.93, 105.0,    0.0, 0.0 },
  { "Global Titans 50",   "globtitans.php", 28.78,  41.6973, 0.0, 0.0 },
  { "Core Dax",           "coredax.php"   , 97.63,  28.0,    0.0, 0.0 }
};

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  Serial.begin(9600);

  myDisplay.setup();

  // Start up the library
  sensors.begin();

  Time.zone(2);

  // initialise wind speed and humidity string
  wind.reserve(MAX_STR_LEN);
  wind = "0 km/h";
  humidity.reserve(MAX_STR_LEN);
  humidity = "0 %";

  //Particle.variable("second", &second, INT);
  // register the cloud function
  Particle.function("toggleDisp", toggleDisp);

  delay(2000);
}

int toggleDisp(String command) {
  if (displayType == MULTI) {
    displayType = TIME;
  }
  else if (displayType == TIME) {
    displayType = STOCK_1;
  }
  else if (displayType == STOCK_1) {
    displayType = STOCK_2;
  }
  else if (displayType == STOCK_2) {
    displayType = STOCK_3;
  }
  else {
    displayType = MULTI;
  }
  return 1;
}

void loop() {
  uint8_t hour = Time.hour();
  uint8_t minute = Time.minute();
  uint8_t second = Time.second();

  // read temperature sensor data every 5s
  if (firstExec || second % 5 == 0) {
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    sensors.requestTemperatures(); // Send the command to get temperatures
    tIn = sensors.getTempCByIndex(0);
  }

  Serial.print("seconds: ");
  Serial.println(second);
  // read web weather data every 5 minutes:
  if (firstExec || (minute % 5 == 0 && second <= 3)) {
    readWebWeather(tOut, wind, humidity);
  }
  // read all web stock data every 5 minutes:
  if (firstExec || ((minute + 1) % 5 == 0 && second <= 3)) {
    for (uint index = 0; index < NUM_STOCKS; index++) {
      readWebStock(stocks[index].webAddr, stocks[index].price, stocks[index].change);
    }
  }
  // publish data every 15 minutes:
  if (minute % 15 == 0 && second <= 2) {
    Particle.publish("tOut", String(round(tOut*10.0)/10.0) + " at " +
    hour + ":" + minute + ":" + second, 60, PRIVATE);
    //Particle.publish("tOut", String(round(tOut*10.0)/10.0), 60, PRIVATE);
    //Particle.publish("wind", wind, 60, PRIVATE);
  }

  myDisplay.updateDisplay(displayType, tIn, tOut, wind, humidity,
    stocks, NUM_STOCKS);

  if (firstExec) firstExec = false;

  delay(1000);
}
