#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#define FIREBASE_HOST "fruitapp-vgu2020.firebaseio.com"
#define FIREBASE_AUTH ""
#define WIFI_SSID "A3.2.2.18"   //Thay wifi và mật khẩu
#define WIFI_PASSWORD "a32152001"


#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <SerialCommand.h>
 
extern "C" {
  #include "user_interface.h"
}
 
#define SPEC_TRG         D6
#define SPEC_ST          D1
#define SPEC_CLK         D2
#define SPEC_VIDEO       A0
#define WHITE_LED        D4
#define LASER_404        D5

#define SPEC_CHANNELS    288 // New Spec Channel
uint16_t data[SPEC_CHANNELS];
char incomingByte;
#define MIN_LED_PWM 130
#define MAX_LED_PWM 250
#define BUFF_SIZE 5
const byte RX = D1;
const byte TX = D2;
int interval=100;
int previousMillis;

SocketIOClient client;
const char* ssid = "A3.2.2.18";  //Wifi name
const char* password = "a32152001";  //Password
 
char host[] = "vguprojectmango.herokuapp.com";  //Your IP address
int port = 80;                  //Default
char namespace_esp8266[] = "esp8266";   
 
//từ khóa extern: dùng để #include các biến toàn cục ở một số thư viện khác. Trong thư viện SocketIOClient có hai biến toàn cục
// mà chúng ta cần quan tâm đó là
// RID: Tên hàm (tên sự kiện
// Rfull: Danh sách biến (được đóng gói lại là chuối JSON)
extern String RID;
extern String Rfull;


 
void readSpectrometer(){

  int delayTime = 3; // delay time
  // digitalWrite(WHITE_LED, 1);
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
//  digitalWrite(LASER_404, 0);
 // digitalWrite(WHITE_LED, 0);
  delayMicroseconds(delayTime);
  
}
char read_serial_data(unsigned int *w_pwr,unsigned int *l_pwr)
{
unsigned int white_pwr=0;//130--250
unsigned int laser_pwr=0;//130--250
  int i=10000;
   if (Serial.available()<=0 ) return -1;
   // read the incoming byte:
   incomingByte = Serial.read();
   if(incomingByte=='$')
   {
      while(i--){
         if(Serial.available()<=0) continue;
          white_pwr= 1;
          break;
      }       
       
       if(i>0)
       {
        *w_pwr= white_pwr;
        digitalWrite(WHITE_LED,white_pwr);
         *l_pwr=laser_pwr;
         incomingByte=='0';
        return 2;
       }else{
        digitalWrite(WHITE_LED,0);
         white_pwr= 0;
         *w_pwr= white_pwr;
         *l_pwr=laser_pwr;
         incomingByte=='0';
          return -1; 
       }       
   }else{
    digitalWrite(WHITE_LED,0);
         white_pwr= 0;
         *w_pwr= white_pwr;
         *l_pwr=laser_pwr;
         incomingByte=='0';
         return -1; 
        }
        
}

String getData(uint16_t y[SPEC_CHANNELS]){
  
  String result = String("");
  for (int i = 0; i < (SPEC_CHANNELS); i++){
    result += String(data[i]);
    result += ",";
  }
 Serial.println(result);
 return result;
}


void setup()
{
    //Set desired pins to OUTPUT
    pinMode(SPEC_CLK, OUTPUT);
    pinMode(SPEC_ST, OUTPUT);
    pinMode(LASER_404, OUTPUT);
    pinMode(WHITE_LED, OUTPUT);

    digitalWrite(WHITE_LED, 0);
    digitalWrite(LASER_404, 0);
  
    digitalWrite(SPEC_CLK, HIGH); 
    digitalWrite(SPEC_ST, LOW); 

    
    
    
    Serial.begin(9600);
    delay(2000);
    Serial.println("Wifi setup");
    
 
    Serial.print("Connect to:");
    Serial.println(ssid);
 
    //Connecting to wifi
    WiFi.begin(ssid, password);
 
    //Waiting for my love
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500);
        Serial.print('.');
    }
 
    Serial.println();
    Serial.println(F("Wifi connected!!!"));
    Serial.println(F("ESP8266 IP ADRESS (Socket Client ESP8266): "));
    Serial.println(WiFi.localIP());
    client.connect(host, port, namespace_esp8266);
    if (client.connected()){
      Serial.println("Socket server connected");
      Serial.println("Ayyo!!!!!! Project Mangoooo");
      client.send("hello","baby","Mango is back");
    }
    // Firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop()
{

       if (!client.connected()) 
       {
        Serial.println("Reconnecting....Please wait");
        client.connect(host, port, namespace_esp8266);
        while (!client.connected())
        {
          Serial.print('.');
        }
        Serial.println("Done. Welcome back:>");
        client.send("Let","kill","I'm back");
        }


    if (client.monitor()) {
        if (RID == "getData") {
          pinMode(SPEC_CLK, OUTPUT);
          pinMode(SPEC_ST, OUTPUT);
          pinMode(LASER_404, OUTPUT);
          pinMode(WHITE_LED, OUTPUT);
      
          digitalWrite(WHITE_LED, 0);
          digitalWrite(LASER_404, 0);
        
          digitalWrite(SPEC_CLK, HIGH); // Set SPEC_CLK High
          digitalWrite(SPEC_ST, LOW); // Set SPEC_ST Low
          digitalWrite(WHITE_LED,1);
          readSpectrometer();
          delay(10); 
          readSpectrometer();
          delay(10); 
          readSpectrometer();
          delay(10); 
          readSpectrometer();
          String dataResult = getData(data);
          client.send("spectrum", "data", dataResult);
          digitalWrite(WHITE_LED, 0);

          // Upload data to Firebase
          Firebase.pushString("Storage", dataResult);
          Firebase.setString("Realtime",dataResult);

            // handle error
            if (Firebase.failed()) {
              Serial.print("setting /number failed:");
              Serial.println(Firebase.error());
              return;
      }
      Serial.println(dataResult);
        }


        //in ra serial monitor
        Serial.print(RID);
        Serial.print(' ');
        Serial.println(Rfull);
        delay(10);  
        
    }
    delay(10);
if (millis() - previousMillis > interval) {
    previousMillis = millis();
    client.send("Hello", "World", "123456");
};


}
