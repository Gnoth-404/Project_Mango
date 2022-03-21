 #include <wifiTool.h>
WifiTool wifiTool;

void setup() {

  Serial.begin(115200);
  Serial.println("System started");
  
    if (wifiTool.wifiAutoConnect()==0)
  {
    Serial.println("fail to connect to wifi!!!!");
    wifiTool.runApPortal();
  }
}

void loop() {

}
