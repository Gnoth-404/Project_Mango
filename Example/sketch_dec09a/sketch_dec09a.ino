
#include <math.h>
#define CHANNELS    288

uint16_t average[CHANNELS];
uint16_t correction[145];

int VIDEO=36;
int TRIGGER=13;
int CLK=14;
int START=15;
int ledPin = 27;
int ONpin = 26;

int i, i_ebc, i_turb, i_col, j, k, r, b, smoothtemp1, smoothtemp2;
int EBC, TURB, COL;
float aveabs = 0;
char numb='0';
String reading = "";
const int freq = 5000;
const int ledChannel0 = 0;
const int ledChannel1 = 1;
const int resolution = 8;


void setup() {
ledcSetup(ledChannel0, freq, resolution);
ledcSetup(ledChannel1, freq, resolution);

pinMode(CLK, OUTPUT);
pinMode(START, OUTPUT);
pinMode(A0, INPUT);
pinMode(ledPin, INPUT);
pinMode(ONpin, INPUT);

digitalWrite(CLK, HIGH);
digitalWrite(START, LOW);
ledcAttachPin(ledPin, ledChannel0);
ledcAttachPin(ONpin, ledChannel1);

ledcWrite(ledChannel1, 10);
ledcWrite(ledChannel0, 6);

Serial.begin(115200);


} 

void loop() {
  readSpectrometer();
  delay(2000);
}



void readSpectrometer(){
   //  uint16_t save[5];
    unsigned long accum = 0; 

   memset(average,0,sizeof(average));
   int delayTime = 1;
  for(long j = 0; j < 10; j++){

   // delay time

  // Start clock cycle and set start pulse to signal start
  digitalWrite(CLK, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(CLK, LOW);
  digitalWrite(START, HIGH);
  delayMicroseconds(delayTime);

  digitalWrite(START, HIGH);
  }
  //Sample for a period of time
  for(int i = 0; i < 15; i++){

      digitalWrite(CLK, HIGH);
      delayMicroseconds(delayTime);
      delay(14);
      
    //  delay(1);
    //  delayMicroseconds(100);
      digitalWrite(CLK, LOW);
      delayMicroseconds(delayTime); 
      
  }

  //Set SPEC_ST to low
  digitalWrite(START, LOW);

  //Sample for a period of time
  for(int i = 0; i < 87; i++){

      digitalWrite(CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(CLK, LOW);
      delayMicroseconds(delayTime); 
      
  }

  //One more clock pulse before the actual read
 /* digitalWrite(CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(CLK, LOW);
  delayMicroseconds(delayTime);*/



  //Read from SPEC_VIDEO

  for(int i = 0; i < CHANNELS; i++){

      accum=(average[i]*(j) + analogRead(VIDEO)*10)/(j+1);
      average[i]=accum;
      
      //average[i] += analogRead(VIDEO);
      
      digitalWrite(CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(CLK, LOW);
      delayMicroseconds(delayTime);
        
  }

  //Set SPEC_ST to high


  //Sample for a small amount of time
  for(int i = 0; i < 7; i++){
    
      digitalWrite(CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(CLK, LOW);
      delayMicroseconds(delayTime);
    
  }

  digitalWrite(CLK, HIGH);
  delayMicroseconds(delayTime);
  delay(100);

for(int i = 0; i < CHANNELS; i++){
  average[i] = average[i]/10;
  Serial.print(average[i]);
  Serial.print(",");}
  Serial.print(accum);
  Serial.print("\n");
}
