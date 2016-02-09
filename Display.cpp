#include "Display.h"

void Display::setup() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  myDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  //myDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  //myDisplay->display(); // show splashscreen
  //delay(2000);
  myDisplay->clearDisplay();   // clears the screen and buffer

  // text display tests
  myDisplay->setTextSize(1);
  myDisplay->setTextColor(WHITE);
  myDisplay->setCursor(0,0);
  //myDisplay->print("Connecting..");
  myDisplay->print("Firmware v");
  myDisplay->print(System.version());
  myDisplay->display();
}

void Display::print2digits(int number) {
  if (number >= 0 && number < 10) {
    myDisplay->print('0');
  }
  myDisplay->print(number);
}

String Display::twoDigits(uint8_t number) {
  String result = "00";
  if (number >= 0 && number < 10) {
    result = "0" + number;
  }
  else {
    result = number;
  }
}

void Display::updateDisplay(enum displayType_t type,
  float tIn, float tOut, String wind, String humidity) {

  myDisplay->clearDisplay();   // clears the screen and buffer

  if (type == MULTI) {
    myDisplay->setTextSize(1);
    // display indoor temperature
    myDisplay->setCursor(0,20);
    myDisplay->print(tIn,1);
    myDisplay->print(char(247));
    myDisplay->print("C");
    // display outdoor temperature
    myDisplay->setCursor(60,20);
    myDisplay->print("Out ");
    myDisplay->print(tOut,1);
    myDisplay->print(char(247));
    myDisplay->print("C");
    // display wind speed
    myDisplay->setCursor(0,40);
    myDisplay->print(wind);
    // display humidity
    myDisplay->setCursor(80,40);
    myDisplay->print(humidity);

    // display time
    myDisplay->setCursor(0,0);
    print2digits(Time.hour());
    if (showColon) {
      myDisplay->print(":");
    }
    else {
      myDisplay->print(" ");
    }
    showColon = !showColon;
    print2digits(Time.minute());

    // display date
    myDisplay->setCursor(60,0);
    print2digits(Time.day());
    myDisplay->print("/");
    print2digits(Time.month());
    myDisplay->print("/");
    myDisplay->print(Time.year());
  }
  else {
    myDisplay->setTextSize(4);
    // display time
    myDisplay->setCursor(5,20);
    print2digits(Time.hour());
    if (showColon) {
      myDisplay->print(":");
    }
    else {
      myDisplay->print(" ");
    }
    showColon = !showColon;
    print2digits(Time.minute());
  }

  myDisplay->display();
}
