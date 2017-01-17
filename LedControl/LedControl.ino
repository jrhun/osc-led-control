#include "server.h"

void setup() {
  // put your setup code here, to run once:
  setupWiFi();
  setupLED();
}

void loop() {
  // put your main code here, to run repeatedly:
  loopWiFi();
}
