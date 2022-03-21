// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License
//
// This example shows how to deserialize a JSON document with ArduinoJson.
//
// https://arduinojson.org/v6/example/parser/

#include <ArduinoJson.h>
#include <SPIFFS.h>
void setup() {
  // Initialize serial port
  Serial.begin(115200);
  

      if (!SPIFFS.begin())
    {
      // Serious problem
      Serial.println(F("SPIFFS Mount failed"));

    }
  File schedules = SPIFFS.open("/secrets.json", "r");
  
  if(schedules && schedules.size()) {

    DynamicJsonDocument wifiarray(1300);
    JsonArray array = wifiarray.to<JsonArray>();
    deserializeJson(wifiarray, schedules);

    for (byte i = 0; i < 7; i++) {
          JsonObject obj = array[i];
          String test = obj["ssid"];
          Serial.println("Test : " + test);

       
      schedules.close();

     };
  }
  else {
    Serial.println("Failed to read file.");
  };
}

void loop() {
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any
// deserialization problem.
//
// The book "Mastering ArduinoJson" contains a tutorial on deserialization.
// It begins with a simple example, like the one above, and then adds more
// features like deserializing directly from a file or an HTTP request.
  // Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
