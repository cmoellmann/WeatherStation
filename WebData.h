#pragma once

#include "application.h"

void readWebWeather(float& temperature, String& wind, String& humidity);
void readWebStock(String webAddress, float& price, float& change);
