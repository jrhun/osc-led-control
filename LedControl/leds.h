#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define LED_PIN     2
#define sensorPin   A0

// Information about the LED strip itself
#define NUM_LEDS    60
#define CHIPSET     WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

extern int BRIGHTNESS;
extern int SPEED;
extern bool LEDSON;

void setupLights(){
  delay(3000); //power up delay protection
  //Add leds and set colour correction
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setTemperature( DirectSunlight );

  //setup onboard LED
//  pinMode(LED_BUILTIN, OUTPUT);
}

void loopLights(){
  if (LEDSON) {
      static uint8_t starthue = 0;
      fill_rainbow( leds, NUM_LEDS, --starthue, SPEED);
//      BRIGHTNESS = map(analogRead(sensorPin), 0, 1023, 0, 128);
      FastLED.setBrightness(BRIGHTNESS);
  } else {FastLED.clear();}

  
  FastLED.show();
  FastLED.delay(30);
}

