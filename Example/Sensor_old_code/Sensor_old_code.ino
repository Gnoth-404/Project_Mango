  
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads(0x48);

#define FIREBASE_HOST "fruitapp-vgu2020.firebaseio.com"
#define FIREBASE_AUTH "AnVeBbVtE7m5g2nCMMMEjX3gXpJmRp4dIjDhWAa8"

// wifi settings
#define WIFI_SSID "FreeWifi"
#define WIFI_PASSWORD "dumaysmmda"
#define SPEC_TRG         12
#define SPEC_ST          14
#define SPEC_CLK         13

//#define SPEC_VIDEO       
float Voltage = 0.0;

//#define WHITE_LED        35
//#define LASER_404        34
#define WHITE_LED        33
#define LASER_404        34
FirebaseData firebaseData;
FirebaseJson dataJson;
int16_t adc0;

//#define VIDEO 35
//#define TRIGGER 14
//#define CLK 0
//#define ledPin 12
//#define ONpin 4
//#define START 16

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
#define FirebasePath "/Storage"


void setup() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    //Firebase.reconnectWiFi(true);
    pinMode(SPEC_CLK, OUTPUT);
    pinMode(SPEC_ST, OUTPUT);
    pinMode(LASER_404, OUTPUT);
    pinMode(WHITE_LED, OUTPUT);
    

    digitalWrite(WHITE_LED, HIGH);
  
    digitalWrite(SPEC_CLK, HIGH); 
    digitalWrite(SPEC_ST, HIGH); 
    Serial.begin(115200);
    
  // put your setup code here, to run once:

}
void loop(){
  
}
//void loop() {
//  // put your main code here, to run repeatedly:
//
//
//          digitalWrite(WHITE_LED,1);
// 
//          readSpectrometer();
//          String dataResult = getData(data);
//          pushData(dataResult);
//          delay(5000);
//}
//
//
//
//
//
//
//void readSpectrometer(){
//
//  int delayTime = 1; // delay time
//  digitalWrite(WHITE_LED, HIGH);
//
//  // Start clock cycle and set start pulse to signal start
//  digitalWrite(SPEC_CLK, LOW);
//  delayMicroseconds(delayTime);
//  digitalWrite(SPEC_CLK, HIGH);
//  delayMicroseconds(delayTime);
//  digitalWrite(SPEC_CLK, LOW);
//  digitalWrite(SPEC_ST, HIGH);
//  delayMicroseconds(delayTime);
//
//  //Sample for a period of time
//  for(int i = 0; i < 15; i++){
//
//      digitalWrite(SPEC_CLK, HIGH);
//      delayMicroseconds(delayTime);
//      digitalWrite(SPEC_CLK, LOW);
//      delayMicroseconds(delayTime); 
// 
//  }
//
//  //Set SPEC_ST to low
//  digitalWrite(SPEC_ST, LOW);
//
//  //Sample for a period of time
//  for(int i = 0; i < 85; i++){
//
//      digitalWrite(SPEC_CLK, HIGH);
//      delayMicroseconds(delayTime);
//      digitalWrite(SPEC_CLK, LOW);
//      delayMicroseconds(delayTime); 
//      
//  }
//
//  //One more clock pulse before the actual read
//  digitalWrite(SPEC_CLK, HIGH);
//  delayMicroseconds(delayTime);
//  digitalWrite(SPEC_CLK, LOW);
//  delayMicroseconds(delayTime);
//
//  //Read from SPEC_VIDEO
//  for(int i = 0; i < SPEC_CHANNELS; i++){
//      adc0=ads.readADC_SingleEnded(0);
//      Voltage = (adc0 * 0.1875)/1000;
//      Serial.print("AIN0: "); 
//Serial.print(adc0);
//Serial.print("\tVoltage: ");
//Serial.println(Voltage, 7); 
//Serial.println();
//
//      data[i] = Voltage;
//
//      digitalWrite(SPEC_CLK, HIGH);
//      delayMicroseconds(delayTime);
//      digitalWrite(SPEC_CLK, LOW);
//      delayMicroseconds(delayTime);
//        
//  }
//
//  //Set SPEC_ST to high
//  digitalWrite(SPEC_ST, HIGH);
//
//  //Sample for a small amount of time
//  for(int i = 0; i < 7; i++){
//    
//      digitalWrite(SPEC_CLK, HIGH);
//      delayMicroseconds(delayTime);
//      digitalWrite(SPEC_CLK, LOW);
//      delayMicroseconds(delayTime);
//    
//  }
//
//  digitalWrite(SPEC_CLK, HIGH);
//  delayMicroseconds(delayTime);
//  digitalWrite(WHITE_LED,0);
//  }
//
//
//void pushData(String sensorData){
//  dataJson.set("/Timestamp/.sv","timestamp");
//  dataJson.set("/Data",sensorData);
//  Firebase.push(firebaseData, FirebasePath , dataJson);
//  Firebase.set(firebaseData, "/Realtime", sensorData);
//  dataJson.clear();
//}
