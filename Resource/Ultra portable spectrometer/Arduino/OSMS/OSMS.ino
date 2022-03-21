
#include <math.h>
#define CHANNELS    288
#include <SoftwareSerial.h>

uint16_t average[CHANNELS];
uint16_t correction[145];

int VIDEO=A0;
int TRIGGER=D6;
int CLK=D2;
int START=D1;
int ledPin = D4;
int ONpin = D5;

char read=0; 
int i, i_ebc, i_turb, i_col, j, k, r, b, smoothtemp1, smoothtemp2;
int EBC, TURB, COL;
float aveabs = 0;
char numb='0';
String reading = "";
SoftwareSerial mySerial(D1, D2); //(// RX, TX)

void setup() {

pinMode(CLK, OUTPUT);
pinMode(START, OUTPUT);
pinMode(A0, INPUT);
pinMode(ledPin, INPUT);
pinMode(ONpin, INPUT);

digitalWrite(CLK, HIGH);
digitalWrite(START, LOW);


analogWrite(ONpin, 10);
analogWrite(ledPin, 6);

mySerial.begin(115200);
Serial.begin(115200);

} 

void loop() {

if(mySerial.available()>0||Serial.available()>0){
if(mySerial.available()>0){
{reading = mySerial.readString();

    if(reading.length()>1){numb='6';}
      //Read the incoming data and store it into variable data
    else {
     numb = reading[0]; }}}
 
else if(Serial.available()>0){

  reading=Serial.readString();
  numb=reading[0];}


switch (numb) { //switches between whichever numbers were sent by the mobile application

case '1':
delay(100);
digitalWrite(LED_BUILTIN, HIGH);  //If value is 1 then LED turns ON - (for testing purposes)
break;

case '2':
delay(100);
digitalWrite(LED_BUILTIN, LOW);   //If value is 0 then LED turns OFF
break;

case '3':
delay(100);
CalibrateLED();   //Calls the calibrate LED function
break;

case '4':
delay(100);
GetBaseline();   //Calls the Get Baseline function
break;

case '5':
delay(100);
Capture();   //Calls the Capture function
break;

case '6':
delay(100);
specificWavelength();   //Calls the Capture function
break;

default:
break;
} 
}
}


void Capture(){ // this captures the absorption spectrum when called
aveabs=0;
String spacing = String(", "); //the application reads data in "nm, intensity" format, so a comma and a space is needed

readSpectrometer();
delay(100);// reads the spectrometer again 

String tot = ""; 
smoothtemp1;
smoothtemp2=average[52];

for(i = 53; i < 198; i++){
smoothtemp1 = smoothtemp2;  
smoothtemp2 = average[i];
average[i]=(average[i]+smoothtemp1+average[i+1])/3;

average[i]=average[i]-980; // removes background. background values vary a bit for every measurement, 96 is an average of about 10 measurements
if((average[i]<1)||(average[i]>65000)){average[i]=1;}// corr=6500;} // this makes sure that there is no overflow or division by zero (usually happens when spectra outside of the LED spectrum is registered)

float m = correction[i-53]/(float)average[i];
aveabs += log10(m);

}

aveabs=aveabs/(198-53);

for(i = 53; i < 198; i++){//reads from position 53 to position 198, which corresponds to approximately 450-750nm
uint16_t nm[145];
int corr;

 //nm[i] calculates the corresponding wavelength to each pixel - the parameters are included in the C12880MA manual and are unique, so the above line needs to be corrected accordingly
/*in order to just register and check the spectrum of the light source
comment out lines starting from the "corr=correction[i]" to "float n = log10(m)"
line (included) and substitute "String(n,4)" in the "String val =" with "data[i]"*/

//corr=correction[i]; // this copies the baseline data, so it could be modified without modifying the actual baseline data
//average[i]=average[i]-980; // removes background. background values vary a bit for every measurement, 96 is an average of about 10 measurements
//if((average[i]<1)||(average[i]>65000)){average[i]=1;}// corr=6500;} // this makes sure that there is no overflow or division by zero (usually happens when spectra outside of the LED spectrum is registered)

float m = correction[i-53]/(float)average[i]; //calculates the baseline LED spectrum divided by the spectrum with an inserted sample
float n = log10(m)+0.0556-0.0767*aveabs; // calculates absorbance

float integ = nm[i-53]/10.0; //this is due to the multiplication by 10 at the end of nm[i] calculation. This is used to avoid a float massive, which takes up more space than an int massive - this means that we get for example a calculated position of 550.12345*10 =5501.2345 (which is 5501 as an int) and then we divide it back as a float and get the value of 550.1 nm
String temp = String(integ,1); //turns it into a sting with 1 decimal place
String val = String(n,4); //turns into a string with 4 decimal places


       tot += temp; //adds the resulting nm to a string
       tot += ", "; //adds a comma
       tot += val; //adds the resulting absorbance to a string
       tot+="\n"; // adds a new line
     //  Serial.print(tot);
       mySerial.print(tot); //prints the "nm, absorbance" string
       tot = ""; //erases the contents of the "nm, absorbance" string

         }
    //     mySerial.print(tot);
    //     Serial.print(tot);
    //     tot = "";
delay(200);
/*for(int i = 2; i < CHANNELS; i++){
  float t = correction[i]/(float)average[i];
 // average[i] = average[i]/10;
  Serial.print(t);
  Serial.print(",\n");}*/
}


void CalibrateLED(){
j=0;

/*for(int f = 2; f < 200; f++){ //LED brightness
analogWrite(ledPin, f);

for(int i = 0; i < CHANNELS; i++){
  if(data[i]>950){j=f-1; break;}}
if(j>0){break;}
}*/
analogWrite(ledPin, 6);
delay(100);
mySerial.print(" ");
delay(100);


/*for additional LEDs, uncomment the lines below*/

/*k=0;

for(int f = 2; f < 250; f++){ //LED brightness
analogWrite(UVPin, f);

readSpectrometer();
delay(100);
readSpectrometer();
delay(100);

for(int i = 0; i < CHANNELS; i++){
  if(data[i]>950){k=f-1; break;}}
if(k>0){break;}
}
analogWrite(UVPin, k);*/

}

void GetBaseline(){ //to be multiplied with


readSpectrometer();
delay(100);

for(int i = 53; i < 198; i++){
  
int smooth =(average[i]+average[i-1]+average[i+1])/3;

correction[i-53]=smooth-980;
if((correction[i-53]<1)||(correction[i-53]>65000)){correction[i-53]=1;}

}

mySerial.print(" ");

}

void specificWavelength(){

aveabs=0;
int singlenm, singlei;
readSpectrometer();
delay(100);


int wlnumber=reading.toInt();

for(i = 53; i < 198; i++){
singlenm=(3.103932661*pow(10,2)+2.683934106*i-1.098262279*pow(10,-3)*pow(i,2)-7.817392551*pow(10,-6)*pow(i,3)+9.609636190*pow(10,-9)*pow(i,4)+4.681760466*pow(10,-12)*pow(i,5));
if(singlenm>wlnumber){singlei=i; break;}}

smoothtemp2=average[52];
for(i = 53; i < 198; i++){
smoothtemp1 = smoothtemp2;  
smoothtemp2 = average[i];
average[i]=(average[i]+smoothtemp1+average[i+1])/3;

average[i]=average[i]-980; // removes background. background values vary a bit for every measurement, 96 is an average of about 10 measurements
if((average[i]<1)||(average[i]>65000)){average[i]=1;}// corr=6500;} // this makes sure that there is no overflow or division by zero (usually happens when spectra outside of the LED spectrum is registered)

float m = correction[i-53]/(float)average[i];
aveabs += log10(m);
}
aveabs=aveabs/(198-53);

float m = correction[singlei-53]/(float)average[singlei]; //calculates the baseline LED spectrum divided by the spectrum with an inserted sample
m = log10(m)+0.0556-0.0767*aveabs;

m =m*100;
m=m/100;
mySerial.print(m);

}


void readSpectrometer(){
 //  uint16_t save[5];
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
}
 for(int i = 0; i < CHANNELS; i++){
//  average[i] = average[i]/10;
  Serial.print(average[i]);
  Serial.print(",");}
//  Serial.print(accum);
  Serial.print("\n");

}
