#include <Adafruit_NeoPixel.h>

#define PIN 6
#define COUNT_PER_ROW 60
#define STRIP_COUNT 3
const uint16_t LED_COUNT = COUNT_PER_ROW * STRIP_COUNT;


uint8_t red = 1;
uint8_t green = 1;
uint8_t blue = 1;

uint8_t randomRed = 1;
uint8_t randomBlue = 1;
uint8_t randomGreen = 1;

const uint16_t MAX_DELAY = 50;
const uint16_t MIN_DELAY = 5;

const uint32_t MAX_WHEEL_CYLE = 5;
const uint32_t MIN_WHEEL_CYLE = 0;
uint16_t currentWheelCycle = 1;
uint8_t currentWheelDirection = 1;

uint16_t currentDelay = MIN_DELAY;
int8_t currentDelayDirection = 5;

//TODO: switch this on and off with user input
bool syncAcrossAll = true;

//for now, assume we'll hook it up in a sane configuration where every alternate row is reversed
bool alternateRowsReversed = true;

//TODO: standardize usage of variable names
//TOOD: create custom typedef for strip color
//TODO: adjust brightness programatically with strip.setBrightness

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //randomSeed(analogRead(0));
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  //colorCycle(50);
  //rainbow(50);
  //fadeIntoColor(strip.Color(255, 0, 0), strip.Color(0,255,0), 20);
  rainbow(50);
//  colorWipeFromMiddle(strip.Color(255, 0, 0), 50); // Red
//  colorWipeFromMiddle(strip.Color(0, 255, 0), 50); // Green
//  colorWipeFromMiddle(strip.Color(255, 0, 255), 50); // Purple
//  colorWipeFromMiddle(strip.Color(0, 255, 255), 50); // Yellow
//  colorWipeFromMiddle(strip.Color(0, 0, 255), 50); // Blue
  
  //rainbow(20);
  //rainbowCycle(20);
  //rainbowCycleWithDirectionChange();
  
  
}

void colorCycle(uint8_t wait) {
  //for (uint16_t i=0; i < strip.numPixels(); i++) {
  for (uint16_t i=0; i < COUNT_PER_ROW; i++) {
    uint32_t color = strip.Color(red, green, blue);
    strip.setPixelColor(i, color);
    strip.show();
    incrementColors();
  }
  delay(wait);
}

void flicker(uint32_t c, uint8_t wait) {
  
}

void incrementColors() {
  red = (red + 1) % 255;
  green = (green + 3)  % 255;
  blue = (blue + 5) % 255;
}

void randomizeColors() {
  randomRed = random(0, 255);
  randomGreen = random(0, 255);
  randomBlue = random(0, 255);
}

void randomFreakout(uint8_t wait) {
  for (uint16_t i=0; i < strip.numPixels(); i++) {
    randomizeColors();
    uint32_t color = strip.Color(randomRed, randomGreen, randomBlue);
    setPixelColor(i, color, false);
    strip.show();
  }
  delay(wait);
}

//(strip.Color(127, 0, 0), 50); // Red
//colorWipe(strip.Color(, 127, 0), 50); // Green
//colorWipe(strip.Color(0, 0, 255), 50); // Blue
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      setPixelColor(i, c, true);
      strip.show();
      delay(wait);
  }
}

void colorWipeFromMiddle (uint32_t c, uint8_t wait) {
  uint8_t rowTotal = COUNT_PER_ROW;
  for(uint16_t i= 0; i < rowTotal / 2; i++) {
      setPixelColor(rowTotal / 2 - i, c, true);
      setPixelColor(rowTotal / 2 + 1 + i, c, true);
      strip.show();
      delay(wait);
  }
}

void fadeIntoColor(uint32_t startColor, uint32_t finishColor, uint8_t wait) {
  Serial.println(unpackRedValue(startColor));
  Serial.println(unpackGreenValue(startColor));
  Serial.println(unpackBlueValue(startColor));
  Serial.println(unpackRedValue(finishColor));
  Serial.println(unpackGreenValue(finishColor));
  Serial.println(unpackBlueValue(finishColor));
}

//rainbow(20);
void rainbow(uint8_t wait) {
  uint16_t i, j;
  uint32_t color;
  
  for(j=0; j<256; j++) {
    for(i=0; i < COUNT_PER_ROW; i++) {
      color = Wheel((i+j) & 255);
      setPixelColor(i, color, true);
    }
    strip.show();
    Serial.println(j);
    delay(wait);
  }
}

void setPixelColor(uint8_t i, uint32_t color, bool syncAcrossAll) {
  if (syncAcrossAll) {
    setNthPixelOfEachRowToColor(i,color);
  } else {
    strip.setPixelColor(i, color);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycleWithDirectionChange() {
  uint16_t i, j;
  uint32_t color;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< COUNT_PER_ROW; i++) {
      color = Wheel(((i * 256 / COUNT_PER_ROW) + j) & 255);
      setPixelColor(i, color, true);
    }
    strip.show();
    delay(5);
  }
  
  for(j=256 * 5; j > 0; j--) { // 5 cycles of all colors backwards
    for(i=strip.numPixels(); i > 0; i--) {
      color = Wheel(((i * 256 / COUNT_PER_ROW) + j) & 255);
      setPixelColor(i, color, true);
    }
    strip.show();
    delay(5);
  }
}

void changeDelay() {
  currentDelay += currentDelayDirection;  
  if (currentDelay <= MIN_DELAY || currentDelay >= MAX_DELAY) {
    //reverse delay direction
    Serial.println("Reversing delay direction");
    currentDelayDirection *= -1;
  }
}

//rainbowCycle(20);
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  uint32_t color;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< COUNT_PER_ROW; i++) {
      //color = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
      color = Wheel(((i * 256 / COUNT_PER_ROW) + j) & 255);
      setPixelColor(i ,color, true);
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setNthPixelOfEachRowToColor(uint8_t n, uint32_t color) {
  for (uint16_t i = 0; i < STRIP_COUNT; i++) {
    uint16_t j;
    if (alternateRowsReversed && (i % 2 == 1))  {
      //we're on a reversed row
      j = ((i + 1) * COUNT_PER_ROW) - n; // 63 would become 117
    } else {
      j = (i * COUNT_PER_ROW) + n;
    }
    strip.setPixelColor(j, color); 
  }
  return;
}

//working with shifted bits, so this is necessary to get a color from a combination of them
uint8_t unpackRedValue(uint32_t color) {
  return (uint8_t)(color >> 16);
}
uint8_t unpackGreenValue(uint32_t color) {
  return (uint8_t)(color >> 8);
}
uint8_t unpackBlueValue(uint32_t color) {
  return (uint8_t)(color);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t FastWheel(byte WheelPos) {
  if(WheelPos < 122) {
   return strip.Color(WheelPos * 2, 255 - WheelPos * 2, 0);
  } else {
   WheelPos -= 122;
   return strip.Color(0, WheelPos * 2, 255 - WheelPos * 2);
  }
}
