#include "Stocks.h"

void getSumData(stock_t stocks[], float& entryValSum, float& marketValSum,
  float& relProfit) {
  marketValSum = 0.0;
  entryValSum = 0.0;
  for (uint index = 0; index < NUM_STOCKS; index++) {
    float marketVal = stocks[index].price * stocks[index].quantity;
    marketValSum = marketValSum + marketVal;
    float entryVal = stocks[index].entryPrice * stocks[index].quantity;
    entryValSum = entryValSum + entryVal;
  }
  float profit = marketValSum - entryValSum;
  relProfit = profit / entryValSum * 100.0;
}
