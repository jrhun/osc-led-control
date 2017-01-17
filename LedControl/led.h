#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t PixelCount = 60; // make sure to set this to the number of pixels in your strip

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
//
// NeoEsp8266Uart800KbpsMethod uses GPI02 instead

NeoGamma<NeoGammaTableMethod> colorGamma;

const uint8_t c_MinBrightness = 8; 
const uint8_t c_MaxBrightness = 255;

void setupLED()
{
  strip.Begin();
  strip.ClearTo(RgbColor(20,20,128));
  strip.Show();
  Serial.print("Leds Setup!\n");
}


void setHSL(HslColor setColor) 
{
  // Sets the whole LED strip to setColor
  for (uint16_t index = 0; index < strip.PixelCount(); index++) 
  {
    RgbColor color = RgbColor(setColor);
    color = colorGamma.Correct(color);
    strip.SetPixelColor(index, color);
  }
  strip.Show();
}

