#include <stdlib.h>

#include <Adafruit_NeoPixel.h>

// There are two flowersets; each has one pin for each color.
// These pins must be capable of doing PWM.
#define FS1R 9
#define FS1G 10
#define FS1B 11

#define FS2R 3
#define FS2G 5
#define FS2B 6

// There is also a 60-pixel NeoPixel strip.
#define STRIP 7
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, STRIP, NEO_GRB+NEO_KHZ800);


#define HALLOWEEN

// Each loop, any unlit pixel will light itself with this probability.
#ifdef HALLOWEEN
#define SPARKLE_PROBABILITY 0.003
#else
#define SPARKLE_PROBABILITY 0.01
#endif

// A higher SPARKLE_RATE means faster sparkles.
#ifdef HALLOWEEN
#define SPARKLE_RATE 1
#else
#define SPARKLE_RATE 2
#endif

#define SPARKLE_STEP SPARKLE_RATE*2

// Brightness at the start of a sparkle. Must be odd.
#define SPARKLE_START SPARKLE_STEP+1

// Maximum brightness of a sparkle.
#ifdef HALLOWEEN
#define SPARKLE_PEAK 128
#else
#define SPARKLE_PEAK 64
#endif

// Some of the pixels are literally hanging out of the vase.
// We'll just ignore those.
#define SPARKLE_SKIP 12

// We'll "cycle" from 0 to 255 and back, cycle_step at a time.
// This will be used to update the flowers.
uint8_t cycle = 0;
uint8_t cycle_step = 1;

// We want to run more than one loop in the time it takes for one cycle.
#define TICKS_PER_CYCLE 3
uint8_t ticks = 0;

void updateCycle();
void updateFlowerset1();
void updateFlowerset2();
void updateStrip();

void setup() {
  uint16_t i;
  
  // All of the flowerset color pins will be PWMed output.
  pinMode(FS1R, OUTPUT);
  pinMode(FS1G, OUTPUT);
  pinMode(FS1B, OUTPUT);
  pinMode(FS2R, OUTPUT);
  pinMode(FS2G, OUTPUT);
  pinMode(FS2B, OUTPUT);

  strip.begin();
  strip.show();
}

void loop() {
  updateCycle();
  updateFlowerset1();
  updateFlowerset2();
  updateStrip();

  delay(20);
}

void updateCycle() {
  ticks += 1;
  if (ticks < TICKS_PER_CYCLE) {
    return;
  } else {
    ticks = 0;
  }
  
  cycle += cycle_step;

  // If we've hit the max or min for a cycle, reverse the direction.
  if (cycle == 0 || cycle == 255) {
    cycle_step = -cycle_step;
  }
}

// Flowerset 1 will cycle from white to bluish and back.
void updateFlowerset1() {
  analogWrite(FS1R, 255 - cycle);
  analogWrite(FS1G, 255 - cycle);
  analogWrite(FS1B, 255);
}


void updateFlowerset2() {
  analogWrite(FS2R, 255);
  analogWrite(FS2G, cycle);
  analogWrite(FS2B, cycle);
}

void updateStrip() {
  uint32_t color;
  uint8_t value;
  uint16_t i;
  uint16_t n = strip.numPixels();
  for (i = SPARKLE_SKIP; i < n; ++i) {
    color = strip.getPixelColor(i);
    value = (color >> 16) & 0xff;

    if ((value == 0) && (rand() < SPARKLE_PROBABILITY * RAND_MAX)) {
      // If this pixel is off, maybe we should turn it on.
      #ifdef HALLOWEEN
      strip.setPixelColor(i, SPARKLE_START, 0, 0);
      #else
      strip.setPixelColor(i, SPARKLE_START, SPARKLE_START, SPARKLE_START);
      #endif
    } else {
      // If the single-color value is odd, we're going up.
      if (value % 2 == 1) {

        if (SPARKLE_STEP > (SPARKLE_PEAK - value)) {
          // If stepping it up would be too much, let's start stepping down.
          value -= 1;
        } else {
          // Otherwise, keep steppin' it up!
          #ifdef HALLOWEEN
          strip.setPixelColor(i, value+SPARKLE_STEP, 0, 0);
          #else
          strip.setPixelColor(i, value+SPARKLE_STEP, value+SPARKLE_STEP, value+SPARKLE_STEP);
          #endif
        }
      }

      // If the single-color value is even at this point, we're going down.
      if (value % 2 == 0) {

        if (SPARKLE_STEP > value) {
          strip.setPixelColor(i, 0, 0, 0);
        } else {
          #ifdef HALLOWEEN
          strip.setPixelColor(i, value-SPARKLE_STEP, 0, 0);
          #else
          strip.setPixelColor(i, value-SPARKLE_STEP, value-SPARKLE_STEP, value-SPARKLE_STEP);
          #endif
        }
      }
    }
  }

  strip.show();
}
