#include "WebData.h"
#include <ArduinoJson.h>

void readWebData(float& temperature, String& wind, String& humidity)
{
  TCPClient client;
  //String server = "beton-radio.de";
  IPAddress server(192, 168, 1, 27);
  int n = 0; // char counter
  uint timeout = 0;
  bool foundJsonStart = false;
  bool foundJsonEnd = false;
  char json[80] = "";

  // TCP client connect
  Serial.println("connecting....");
  Serial.println(server);
  if (client.connect(server, 80))
  {
    Serial.println("connected");
    client.println("GET /weather.php HTTP/1.1");
    //client.println("Host: www.beton-radio.de");
    client.println("Host: 192.168.1.27");
    client.println("Content-Length: 0");
    client.println();
  }
  else
  {
    Serial.println("connection failed");
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
     return;
   }

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
