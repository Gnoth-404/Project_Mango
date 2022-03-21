#define CHANNELS    288

uint16_t average[CHANNELS];

#define VIDEO 35
#define TRIGGER 14
#define CLK 0
#define ledPin 12
#define ONpin 4
#define START 16





void setup() {


pinMode(CLK, OUTPUT);
pinMode(START, OUTPUT);
pinMode(A0, INPUT);
pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, HIGH);
pinMode(ONpin, OUTPUT);
digitalWrite(CLK, HIGH);
digitalWrite(START, LOW);


Serial.begin(115200);


} 

void loop() {
  readSpectrometer();
  delay(5000);
}





void readSpectrometer(){
 unsigned long accum = 0;
 memset(average,0,sizeof(average));

 
  for(long j = 0; j < 10; j++){

  int delayTime = 1; // delay time

  // Start clock cycle and set start pulse to signal start
  digitalWrite(CLK, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(CLK, LOW);
  digitalWrite(START, HIGH);
  delayMicroseconds(delayTime);

  digitalWrite(START, HIGH);
  
  //Sample for a period of time
  for(int i = 0; i < 15; i++){

    digitalWrite(CLK, HIGH);
    delayMicroseconds(delayTime);
    delay(14);

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

  digitalWrite(CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(CLK, LOW);
  delayMicroseconds(delayTime);

  for(int i = 0; i < CHANNELS; i++){

      //accum=(average[i]*(j) + analogRead(VIDEO)*10)/(j+1);
      //average[i]=accum;
      
      average[i] += analogRead(VIDEO);
      
      digitalWrite(CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(CLK, LOW);
      delayMicroseconds(delayTime);
  }

  //Set SPEC_ST to high
    digitalWrite(START, LOW);


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
}


String result= String("");
for(int i = 0; i < CHANNELS; i++){
 average[i] = average[i]/10;
 //Serial.print(average[i]);
 result+=String(average[i]);
 result+=",";
//  Serial.print(",");}
//  Serial.print(accum);
//  Serial.print("\n");
}

Serial.println(result);
}
