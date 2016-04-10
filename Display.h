#pragma once

#include "application.h"
#include "Adafruit_SSD1306.h"
#include "Stocks.h"

#define OLED_RESET D4

enum displayType_t { TIME, MULTI, STOCK_1, STOCK_2, STOCK_3 };

class Display {
public:
  Display(Adafruit_SSD1306* display) : myDisplay(display) {}
  void updateDisplay(enum displayType_t type,
    float tIn, float tOut, String wind, String humidity,
    stock_t stocks[], uint numStocks);
  void setup();

private:
  void print2digits(int number);
  String twoDigits(uint8_t number);

  Adafruit_SSD1306* myDisplay;
  bool showColon = false;
};
