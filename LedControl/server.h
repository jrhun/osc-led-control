
/* Starts up a wifi server on the ESP8266, listening on "localPort" for UDP packets.
 *  
 */

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include <OSCBoards.h>
#include <OSCBundle.h>

const char* ssid     = "The House of Benjamin";
const char* password = "bellasatonthemat";
unsigned int localPort = 10000;      

WiFiUDP udp;

void routeTone(OSCMessage &msg);

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
   OSCBundle bundleIN;
   int size;
 
   if( (size = Udp.parsePacket())>0)
   {
     while(size--)
       bundleIN.fill(Udp.read());

      if(!bundleIN.hasError())
        bundleIN.route("/tone", routeTone);
   }
}

