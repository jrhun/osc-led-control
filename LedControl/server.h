/* Starts up a wifi server on the ESP8266, listening on "localPort" for UDP packets.
 *  
 */

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

//#include <OSCBoards.h>
#include <OSCBundle.h>
#include <OSCMessage.h>

#include "led.h"

const char* ssid     = "*************";
const char* password = "************";

unsigned int localPort = 10000;      // local port to listen for UDP packets
unsigned int outPort = 12000;
IPAddress outIP(192, 168, 0, 100);  //TouchOSC IP for visual feedback

WiFiUDP udp;

void routeHSL(OSCMessage &msg, int addrOffset);
void routeColor(OSCMessage &msg, int addrOffset);
void updateRgbSlider(HslColor color);
void sendOSC(OSCMessage &msgOUT);
extern bool animationOn;
extern uint8_t gCurrentAnimationNumber;

int resolution = 4; //default resolution for encoders

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
      {
        //Serial.print("valid OSC!");
        msgIN.route("/HSV", routeHSL);
        msgIN.route("/colour", routeColor);
//        Serial.print("hmmm");
        if (msgIN.fullMatch("/fx1"))
        {
          animationOn = true; 
          gCurrentAnimationNumber = 0;
        }
        if (msgIN.fullMatch("/fx2"))
        {
          animationOn = true; 
          gCurrentAnimationNumber = 1;
        }
        if (msgIN.fullMatch("/fx4"))
        {
          animationOn = false; 
        }
        
        if (msgIN.fullMatch("/resolution")) 
        {
          int resolution = int(msgIN.getFloat(0)*49.0) + 1;    // resolution from 1-50
          OSCMessage msgOUT("/label/resolution");
          msgOUT.add(resolution);
          sendOSC(msgOUT);
        }
      }
        
   }
}

void routeHSL(OSCMessage &msg, int addrOffset) 
{
    // for now if you just change one value, it uses first pixel as a reference point
    HslColor currentColor = HslColor(baseColor);
    // currentColor contains the floats H, S, L
  
   if (msg.fullMatch("/lightness", addrOffset))
   {
      currentColor.L = msg.getFloat(0);
      //update labels
      OSCMessage msgOUT("/labels/lightness");
      msgOUT.add(currentColor.L * 512.0); 
      sendOSC(msgOUT);
   } 
   else if (msg.fullMatch("/hue", addrOffset)) 
   {
      //Hue is sent on an encoder. Use increment amount
      int hue = int(currentColor.H * 1000.0);
      hue = hue + (int(msg.getFloat(0)) * resolution); 
      hue %= 1000;       //keep within 1000 if we wrap around
      if (hue < 0) {hue = 1000 + hue;}
      currentColor.H = float(hue) / 1000;
      
      //update labels - to be moved to centralised section
      OSCMessage msgOUT("/labels/hue");
      msgOUT.add(currentColor.H * 255.0); 
      sendOSC(msgOUT);
      
   }
   else if (msg.fullMatch("/saturation", addrOffset))
   {
      currentColor.S = msg.getFloat(0);
      
      OSCMessage msgOUT("/labels/saturation");
      msgOUT.add(currentColor.S * 255.0); 
      sendOSC(msgOUT);
   }
   setHSL(currentColor);
   updateRgbSlider(currentColor);
}

void routeColor(OSCMessage &msg, int addrOffset) 
{
//    Serial.print("We are in routeColor");
    if (msg.fullMatch("/red", addrOffset))    { strip.ClearTo(RgbColor(255,0,0));}
    if (msg.fullMatch("/blue", addrOffset))   { strip.ClearTo(RgbColor(0,0,255));}
    if (msg.fullMatch("/green", addrOffset))  { strip.ClearTo(RgbColor(0,255,0));}
    if (msg.fullMatch("/pink", addrOffset))   { strip.ClearTo(RgbColor(244,66,228));}
    if (msg.fullMatch("/orange", addrOffset)) { strip.ClearTo(RgbColor(244,137,66));}
    if (msg.fullMatch("/purple", addrOffset)) { strip.ClearTo(RgbColor(128,66,244));}
    if (msg.fullMatch("/white", addrOffset))  { strip.ClearTo(RgbColor(128,128,128));}
}

void updateRgbSlider(HslColor color) 
{
  RgbColor rgbColor(color);
  OSCMessage msgOUT("/RGB");
  float R,G,B;
  R = rgbColor.R / 255.0;
  G = rgbColor.G / 255.0;
  B = rgbColor.B / 255.0;  
  msgOUT.add(R);    // order is important
  msgOUT.add(G);
  msgOUT.add(B);
  sendOSC(msgOUT);
}

void sendOSC(OSCMessage &msgOUT)
{
  udp.beginPacket(outIP, outPort);
    msgOUT.send(udp);   // send the bytes to udp
  udp.endPacket();      // mark the end of the packet
  msgOUT.empty();       // free space ocupied by message
}
