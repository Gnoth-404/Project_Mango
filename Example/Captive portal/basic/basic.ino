

// Access point library
#include <wifiTool.h>
WifiTool wifiTool;

#define Firebasename "Tienanh"


void setup() {

  Serial.begin(115200);
  Serial.println("System started");
  if (Firebasename == ""){
    Serial.println("No empty");    
    };
  
  
}

void loop() {

}
