#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <SerialCommand.h, a
 
//include thư viện để kiểm tra free RAM trên con esp8266
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
 
SoftwareSerial mySerial(RX, TX, false, 256); 
 
SocketIOClient client;
const char* ssid = "406";          //Tên mạng Wifi mà Socket server của bạn đang kết nối
const char* password = "cc159p406";  //Pass mạng wifi ahihi, anh em rãnh thì share pass cho mình với.
 
char host[] = "192.168.0.104";  //Địa chỉ IP dịch vụ, hãy thay đổi nó theo địa chỉ IP Socket server của bạn.
int port = 3484;                  //Cổng dịch vụ socket server do chúng ta tạo!
char namespace_esp8266[] = "esp8266";   //Thêm Arduino!
 
//từ khóa extern: dùng để #include các biến toàn cục ở một số thư viện khác. Trong thư viện SocketIOClient có hai biến toàn cục
// mà chúng ta cần quan tâm đó là
// RID: Tên hàm (tên sự kiện
// Rfull: Danh sách biến (được đóng gói lại là chuối JSON)
extern String RID;
extern String Rfull;

void validation_data(unsigned char *data,char MIN_,char MAX_)
{
  if(*data<MIN_) *data =0;
  if(*data>MAX_) *data =MAX_;
}
 
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
  
    digitalWrite(SPEC_CLK, HIGH); // Set SPEC_CLK High
    digitalWrite(SPEC_ST, LOW); // Set SPEC_ST Low
    //Bật baudrate ở mức 57600 để giao tiếp với máy tính qua Serial
    Serial.begin(115200);
    mySerial.begin(115200); //Bật software serial để giao tiếp với Arduino, nhớ để baudrate trùng với software serial trên mạch arduino
    delay(10);
 
    //Việc đầu tiên cần làm là kết nối vào mạng Wifi
    Serial.print("Ket noi vao mang ");
    Serial.println(ssid);
 
    //Kết nối vào mạng Wifi
    WiFi.begin(ssid, password);
 
    //Chờ đến khi đã được kết nối
    while (WiFi.status() != WL_CONNECTED) { //Thoát ra khỏi vòng 
        delay(500);
        Serial.print('.');
    }
 
    Serial.println();
    Serial.println(F("Da ket noi WiFi"));
    Serial.println(F("Di chi IP cua ESP8266 (Socket Client ESP8266): "));
    Serial.println(WiFi.localIP());
 
    if (!client.connect(host, port, namespace_esp8266)) {
        Serial.println(F("Ket noi den socket server that bai!"));
        return;
    }
 
    Serial.println("Da san sang nhan lenh");
    
}
 
void loop()
{
 
    //Khi bắt được bất kỳ sự kiện nào thì chúng ta có hai tham số:
    //  +RID: Tên sự kiện
    //  +RFull: Danh sách tham số được nén thành chuỗi JSON!
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
 
    //Kết nối lại!
    if (!client.connected()) {
      client.connect(host, port, namespace_esp8266);
    }
}
