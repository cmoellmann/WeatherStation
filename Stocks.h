#pragma once

#include "application.h"

#define NUM_STOCKS 3

struct stock_t {
  String name;
  String webAddr;
  float entryPrice;
  float quantity;
  float price;
  float change;
};

void getSumData(stock_t stocks[], float& entryValSum, float& marketValSum,
  float& relProfit);
