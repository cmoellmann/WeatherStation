#include "Display.h"
#include "Stocks.h"

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
  float tIn, float tOut, String wind, String humidity,
  stock_t stocks[], uint numStocks) {

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
  else if (type == TIME) {
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
  else if (type == STOCK_TOTAL) {
    myDisplay->setTextSize(1);
    myDisplay->setCursor(0,0);

    float entryValSum;
    float marketValSum;
    float relProfit;

    getSumData(stocks, entryValSum, marketValSum, relProfit);

    // sum of entry values
    myDisplay->setCursor(0,0);
    myDisplay->print(entryValSum, 2);

    // sum of market values
    myDisplay->setCursor(60,0);
    myDisplay->print(marketValSum, 2);

    // profit %
    myDisplay->setCursor(0,20);
    myDisplay->print(relProfit, 2);
    myDisplay->print("%");
  }
  else {
    uint index = type-STOCK_1;

    if (index < 0 || index > numStocks-1) {
      Serial.println("ERROR: invalid stock index");
      index = 0;
    }

    // stock name
    myDisplay->setTextSize(1);
    myDisplay->setCursor(0,0);
    myDisplay->print(stocks[index].name);

    // last price
    myDisplay->setCursor(0,20);
    myDisplay->print(stocks[index].price,2);

    // change %
    myDisplay->setCursor(60,20);
    myDisplay->print(stocks[index].change, 2);
    myDisplay->print("%");

    // market value
    myDisplay->setCursor(0,40);
    float marketValue = stocks[index].price * stocks[index].quantity;
    myDisplay->print(marketValue, 2);

    // profit %
    myDisplay->setCursor(60,40);
    float entryValue = stocks[index].entryPrice * stocks[index].quantity;
    float profit = marketValue - entryValue;
    myDisplay->print(profit / entryValue * 100.0, 2);
    myDisplay->print("%");
  }

  myDisplay->display();
}
