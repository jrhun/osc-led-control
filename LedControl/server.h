#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <OSCMessage.h>

//code to starts up the server on the ESP8266, listening on UDP port localPort

const char* ssid     = "*************";
const char* password = "************";

unsigned int localPort = 10000;      // local port to listen for UDP packets
unsigned int outPort = 9999;

WiFiUDP udp;

unsigned long lastMsgTime = 0;        //only receive message every 50ms to not overload netcode
const long netFramerate = 100;          //OSC message update rate 100ms = 10hz
unsigned long lastUDPcheck = 0;

extern int BRIGHTNESS = 80;
extern int SPEED = 10;
extern bool LEDSON = true;

void oscParse(OSCMessage &msgIn);

void setupWIFI() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
}

void restartWiFi() {
  udp.begin(localPort);
}

void LEDcontrol (OSCMessage &msg){
//  Serial.println("We got to line LEDcontrol()");
  int ledControl;
  if (msg.isFloat(0)){  
//    Serial.println("Message is float");
    ledControl = int(msg.getFloat(0));
//    Serial.println(ledControl);
//    digitalWrite(LED_BUILTIN, (ledControl > 0)? LOW: HIGH);
  }// (int(msgIn.getFloat(0))
}

void loopServer(){
  OSCMessage msgIn;
  unsigned long currentMillis = millis();
  int packetSize = udp.parsePacket();
  if (packetSize) {
    if (currentMillis - lastMsgTime >= netFramerate) {
      lastMsgTime = currentMillis;
      while (packetSize--){
        msgIn.fill(udp.read());
      }
      // send packets to be parsed
      oscParse(msgIn);
    } else {udp.flush();} //discard data if we are not ready to read yet
  }
//  if ( ( (currentMillis / 1000) % 5) < 1 ) { //check port is still open every 5 seconds
//    if (udp.localPort() != localPort) {
//      setupWiFi();
//    }
//  }
}


void oscParse(OSCMessage &msgIn) {
  // takes an already full msgIn and parses it
  
  if (!msgIn.hasError()) {
    if (msgIn.fullMatch("/leds")) {
      if (int(msgIn.getFloat(0)) == 1) {LEDSON = true;}
      else if (int(msgIn.getFloat(0)) == 0) {LEDSON = false;}
      
    } else if (msgIn.fullMatch("/brightness")) {
      BRIGHTNESS = (int(msgIn.getFloat(0) * 255));
      
    } else if (msgIn.fullMatch("/speed")) {
      SPEED = 35 - (int(msgIn.getFloat(0) * 30));
    } 
//    else if (msgIn.fullMatch("/fxtype")) {
//      // Insert code for different fxs
//    }
  }
}



