#include "src/ProjectI2c_menu.h"
#include <WiFi.h>
#include <FirebaseESP32.h>
#define FIREBASE_HOST "fruitapp-vgu2020.firebaseio.com"
#define FIREBASE_AUTH "AnVeBbVtE7m5g2nCMMMEjX3gXpJmRp4dIjDhWAa8"

// wifi settings
#define WIFI_SSID "A3.2.2.18"
#define WIFI_PASSWORD "a32152001"
#define SPEC_CHANNELS    288 // New Spec Channel
uint16_t data[SPEC_CHANNELS];
String getData(uint16_t y[SPEC_CHANNELS]){
  
  String result = String("");
  for (int i = 0; i < (SPEC_CHANNELS); i++){
    result += String(data[i]);
    result += ",";
  }
 Serial.println(result);
 return result;
}
//Define Data object
FirebaseData firebaseData;
FirebaseJson dataJson;

//
//#define VIDEO 35
//#define TRIGGER 14
//#define CLK 0
//#define ledPin 12
//#define ONpin 4
//#define START 16
#define FirebasePath "/Storage"

#define SPEC_TRG         14
#define SPEC_ST          16
#define SPEC_CLK         0
#define SPEC_VIDEO       35
#define WHITE_LED        12
#define LASER_404        4




void setup() {
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  pinMode(SPEC_VIDEO, INPUT);
  analogSetPinAttenuation(SPEC_VIDEO, ADC_0db);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

    pinMode(SPEC_CLK, OUTPUT);
    pinMode(SPEC_ST, OUTPUT);
    pinMode(LASER_404, OUTPUT);
    pinMode(WHITE_LED, OUTPUT);

    digitalWrite(WHITE_LED, 0);
    digitalWrite(LASER_404, 0);
  
    digitalWrite(SPEC_CLK, HIGH); 
    digitalWrite(SPEC_ST, LOW); 


Serial.begin(115200);


  
setupMenu();
}

void loop() {
    taskManager.runLoop();
}

void CALLBACK_FUNCTION readspectrometer(int id) {
          digitalWrite(WHITE_LED,1);
          readSpectrometer();
          delay(10); 
          readSpectrometer();
          delay(10); 
          readSpectrometer();
          delay(10); 
          readSpectrometer();
          delay(10);
          String dataResult = getData(data);
          digitalWrite(WHITE_LED, 0);
          pushData(dataResult);
 
}


void pushData(String sensorData){
  dataJson.set("/Timestamp/.sv","timestamp");
  dataJson.set("/Data",sensorData);
  Firebase.push(firebaseData, FirebasePath , dataJson);
  Firebase.set(firebaseData, "/Realtime", sensorData);
  dataJson.clear();
}

void readSpectrometer(){

  int delayTime = 3; // delay time
   digitalWrite(WHITE_LED, 1);
  // Start clock cycle and set start pulse to signal start
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delayTime);

  //Sample for a period of time
  for(int i = 0; i < 15; i++){

      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime); 

  }

  //Set SPEC_ST to low
  digitalWrite(SPEC_ST, LOW);

  //Sample for a period of time
  for(int i = 0; i < 85; i++){

      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime); 
      
  }

  //One more clock pulse before the actual read
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);

  //Read from SPEC_VIDEO
  for(int i = 0; i < SPEC_CHANNELS; i++){

      data[i] = analogRead(SPEC_VIDEO);
      
      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime);
        
  }

  //Set SPEC_ST to high
  digitalWrite(SPEC_ST, HIGH);

  //Sample for a small amount of time
  for(int i = 0; i < 7; i++){
    
      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime);
    
  }

  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(LASER_404, 0);
  digitalWrite(WHITE_LED, 0);
  delayMicroseconds(delayTime);
  
}

void CALLBACK_FUNCTION Captiveportal(int id) {
    // TODO - your menu change code
}
