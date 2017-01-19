#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t PixelCount = 60; // make sure to set this to the number of pixels in your strip

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount);
// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  

NeoGamma<NeoGammaTableMethod> colorGamma;

const uint8_t c_MinBrightness = 8; 
const uint8_t c_MaxBrightness = 255;

extern HslColor baseColor(20,20,128);

// List of animations that can be cycled through, each defined as a separate fn (taken from fastled demoreel)
typedef void (*AnimationsList[])();
void rainbow();
void fade();
AnimationsList gAnimations = { rainbow, fade };
bool animationOn = false;              //accesible from anywhere
uint8_t gCurrentAnimationNumber = 0;   //accesible from anywhere

unsigned long previousMillis = 0;        // will store last time LED was updated
const long framerate = 100;      //1000/30 = 33milis for 30fps animation rate


void setupLED()
{
  strip.Begin();
  strip.ClearTo(baseColor);
  strip.Show();
  Serial.print("Leds Setup!\n");
}


void setHSL(HslColor setColor) 
{
  // Sets the whole LED strip to setColor
  for (uint16_t index = 0; index < strip.PixelCount(); index++) 
  {
    RgbColor color(setColor);
    color = colorGamma.Correct(color);
    strip.SetPixelColor(index, setColor);
  }
//  strip.Show();
}


void loopLed() 
{
  // Main control loop for LEDS
  // Checks if any animations are running, 
  if (animationOn) 
  {
    // animation loops each time it is called?
    unsigned long currentMillis = millis(); 
    if (currentMillis - previousMillis >= 1000 / framerate) 
    {
        // note: delay is handled in the loop here
        previousMillis = currentMillis; 
        // call the animation we want ~30fps
        gAnimations[gCurrentAnimationNumber]();
        // do some periodic updates
    }
  }
  if (strip.IsDirty())        //checks if any pixel changes have occured
  {
    uint16_t i = PixelCount - 1;
    baseColor = strip.GetPixelColor(i); //baseColor is the first pixel
    strip.Show();
  }
}

void rainbow() 
{ 
  int width = 90; // every 30 pixels have the same color
  uint16_t i;
  int hue = int(baseColor.H * 360.0);     //convert to 360
  for(i=0; i<PixelCount; i++) {
    hue += (360 / width) * i;
    hue %= 360;                          // wrap around if over 360
    HslColor newColor(baseColor); 
    newColor.H = float(hue / 360.0);
    strip.SetPixelColor(i, newColor);
  }
//  baseColor = HslColor(strip.GetPixelColor(i));   // set base color to last pixel
}

void fade()
{
  
}

