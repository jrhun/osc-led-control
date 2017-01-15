// Taken from fastled Demoreal!!!



// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { manual, rainbow};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loopPattern(int patternNumber){
    // Call the current pattern function once, updating the 'leds' array
  gPatterns[patternNumber]();
}

void manual(){
  
}

void rainbow(){
  
}

