#include "WebData.h"
#include <ArduinoJson.h>

void readWebWeather(float& temperature, String& wind, String& humidity)
{
  TCPClient client;
  IPAddress server(192, 168, 1, 27);
  int n = 0; // char counter
  uint timeout = 0;
  bool foundJsonStart = false;
  bool foundJsonEnd = false;
  char json[80] = "";
  bool success = true;

  Serial.println("start reading weather data from the web...");

  // TCP client connect
  Serial.println("connecting....");
  Serial.println(server);
  if (client.connect(server, 80))
  {
    Serial.println("connected");
    client.println("GET /weather.php HTTP/1.1");
    //String getStr = String("GET /" + webAddress + " HTTP/1.1");
    //client.println(getStr);
    client.println("Host: 192.168.1.27");
    client.println("Content-Length: 0");
    client.println();
  }
  else
  {
    Serial.println("connection failed");
    success = false;
  }
   // read data
   while (true) {
     if (client.available())
     {
       char c = client.read();
       //Serial.print(c);

       if (c == '{') {
         foundJsonStart = true;
         //Serial.println("Found Json start...");
       }
       if (foundJsonStart) {
         json[n] = c;
         n++;
       }
       if (c == '}') {
         foundJsonEnd = true;
         //Serial.println("Found Json end...");
         //break;
       }
     }

     if (!client.connected())
     {
       Serial.println();
       Serial.println("disconnecting.");
       client.stop();
       Serial.println("exit");
       break;
     }
     //Serial.println("while ..");
     timeout++;
     if (timeout > 10000) {
       Serial.println("timout");
       break;
     }
   }

   Serial.println("json:");
   Serial.println(n);
   Serial.println(json);

   StaticJsonBuffer<200> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(json);
   if (!root.success()) {
     Serial.println("parseObject() failed");
     success = false;
   }

   if (success) {
    // temperature
    if (root["temperature"].is<long>()){
      // stored as long in json (when temperature is e.g. 10.0, it is stored as 10 in json,
      // which will return 0.0 if directly stored into a float
      long temp = root["temperature"].as<long>();
      temperature = (float)temp;
    }
    else {
      // stored as double in json
      temperature = (float)root["temperature"];
    }
    // wind speed
    wind = root["wind"];
    wind.trim();
    // humidity
    humidity = root["humidity"];
    humidity.trim();

    Serial.println(temperature);
    Serial.println(wind);
  }
  else {
    temperature = 0.0;
    wind = "0 km/h";
    humidity = "0 %";
  }
}

void readWebStock(String webAddress, float& price, float& change)
{
  TCPClient client;
  IPAddress server(192, 168, 1, 27);
  int n = 0; // char counter
  uint timeout = 0;
  bool foundJsonStart = false;
  bool foundJsonEnd = false;
  char json[80] = "";
  bool success = true;

  Serial.println("start reading stock data from the web...");

  // TCP client connect
  Serial.println("connecting....");
  Serial.println(server);
  if (client.connect(server, 80))
  {
    Serial.println("connected");
    //client.println("GET /weather.php HTTP/1.1");
    //String webAddress = "weather.php"; //stocks[stockId].webAddr;
    String getStr = String("GET /" + webAddress + " HTTP/1.1");
    client.println(getStr);
    client.println("Host: 192.168.1.27");
    client.println("Content-Length: 0");
    client.println();
  }
  else
  {
    Serial.println("connection failed");
    success = false;
  }
   // read data
   while (true) {
     if (client.available())
     {
       char c = client.read();
       //Serial.print(c);

       if (c == '{') {
         foundJsonStart = true;
         //Serial.println("Found Json start...");
       }
       if (foundJsonStart) {
         json[n] = c;
         n++;
       }
       if (c == '}') {
         foundJsonEnd = true;
         //Serial.println("Found Json end...");
         //break;
       }
     }

     if (!client.connected())
     {
       Serial.println();
       Serial.println("disconnecting.");
       client.stop();
       Serial.println("exit");
       break;
     }
     //Serial.println("while ..");
     timeout++;
     if (timeout > 10000) {
       Serial.println("timout");
       break;
     }
   }

   Serial.println("json:");
   Serial.println(n);
   Serial.println(json);

   StaticJsonBuffer<200> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(json);
   if (!root.success()) {
     Serial.println("parseObject() failed");
     success = false;
   }

   if (success) {
    // value
    if (root["value"].is<long>()){
      // stored as long in json (when temperature is e.g. 10.0, it is stored as 10 in json,
      // which will return 0.0 if directly stored into a float
      long priceLong = root["value"].as<long>();
      price = (float)priceLong;
    }
    else {
      // stored as double in json
      price = (float)root["value"];
    }
    // change
    if (root["change"].is<long>()){
      // stored as long in json (when temperature is e.g. 10.0, it is stored as 10 in json,
      // which will return 0.0 if directly stored into a float
      long changeLong = root["change"].as<long>();
      change = (float)changeLong;
    }
    else {
      // stored as double in json
      change = (float)root["change"];
    }
  }
  else {
    price = 0.0;
    change = 0.0;
  }

  Serial.println(price);
  Serial.println(change);
}
