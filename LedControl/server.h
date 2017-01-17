/* Starts up a wifi server on the ESP8266, listening on "localPort" for UDP packets.
 *  
 */

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include <OSCBoards.h>
#include <OSCBundle.h>
#include <OSCMessage.h>

#include "led.h"

const char* ssid     = "The House of Benjamin";
const char* password = "bellasatonthemat";
unsigned int localPort = 10000;      

WiFiUDP udp;

void routeHSL(OSCMessage &msg, int addrOffset);

void setupWiFi() {
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

void loopWiFi() {
   OSCMessage msgIN;
   int size;
 
   if( (size = udp.parsePacket())>0)
   {
     while(size--)
       //Serial.print("Received Message: ");
       msgIN.fill(udp.read());

      if(!msgIN.hasError())
        //Serial.print("valid OSC!");
        Serial.println();
        msgIN.route("/HSV", routeHSL);
   }
}

void routeHSL(OSCMessage &msg, int addrOffset) {
  // for now if you just change one value, it uses first pixel as a reference point
  uint16_t firstpixel = 0;
  RgbColor firstPixelColor = strip.GetPixelColor(firstpixel);
  HslColor currentColor = HslColor(firstPixelColor);
  // currentColor contains the floats H, S, L
  Serial.print("Msg is :");
  Serial.print(msg.getFloat(0));
  Serial.println();

 if (msg.fullMatch("/lightness", addrOffset))
 {
  currentColor.L = msg.getFloat(0);
 } 
 else if (msg.fullMatch("/hue", addrOffset)) 
 {
  currentColor.H = msg.getFloat(0);
 }
 else if (msg.fullMatch("/saturation", addrOffset))
 {
  currentColor.S = msg.getFloat(0);
 }
 
 setHSL(currentColor);
}



