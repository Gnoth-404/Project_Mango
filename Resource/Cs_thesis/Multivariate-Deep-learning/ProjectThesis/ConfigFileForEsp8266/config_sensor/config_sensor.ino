#include <SocketIOClient.h>
#include <WiFi.h> 
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

#define BUFF_SIZE 5

 

SocketIOClient client;
const char* ssid = "A3.2.2.18";          //Wifi name
const char* password = "a32152001";  //Password
 
char host[] = "192.168.1.7";  //Your IP address
int port = 3484;                  //Default
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

    
    
    
    Serial.begin(115200);
    mySerial.begin(115200); 
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
        }
        if (RID == "test") {
          client.send("connected", "data", "Connected");
        }

        //in ra serial monitor
        Serial.print(RID);
        Serial.print(' ');
        Serial.println(Rfull);  
        
    }
 
    
}
