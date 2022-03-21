
#define WIFI_SSID "A3.2.2.18"   //Thay wifi và mật khẩu
#define WIFI_PASSWORD "a32152001"


#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <SerialCommand.h>
 
extern "C" {
  #include "user_interface.h"
}
 
#define SPEC_TRG         D3
#define SPEC_ST          D8
#define SPEC_CLK         D6
#define SPEC_VIDEO       A0
#define WHITE_LED        D4
#define LASER_404        D7

#define SPEC_CHANNELS    288 // New Spec Channel
uint16_t data[SPEC_CHANNELS];

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

  int delayTime = 1; // delay time
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
//  digitalWrite(LASER_404, 0);
 // digitalWrite(WHITE_LED, 0);
  delayMicroseconds(delayTime);
  
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
}
 
void loop()
{

 readSpectrometer();
 getData(data);
 delay(7000);
 

}
