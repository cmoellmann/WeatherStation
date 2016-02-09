#ifndef Display_h
#define Display_h

#include "application.h"
#include "Adafruit_SSD1306.h"

#define OLED_RESET D4

enum displayType_t { TIME, MULTI };

class Display {
public:
  Display(Adafruit_SSD1306* display) : myDisplay(display) {}
  void updateDisplay(enum displayType_t type,
    float tIn, float tOut, String wind, String humidity);
  void setup();

private:
  void print2digits(int number);
  String twoDigits(uint8_t number);

  Adafruit_SSD1306* myDisplay;
  bool showColon = false;
};

#endif
