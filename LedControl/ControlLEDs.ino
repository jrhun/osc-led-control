#include "leds.h"
#include "server.h"

void setup() {
  // put your setup code here, to run once:
  setupWIFI();
  setupLights();
}

void loop() {
  // put your main code here, to run repeatedly:
  loopLights();
  loopServer();
}
