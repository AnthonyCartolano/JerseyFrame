#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
#define NUMPIXELS1 10
#define NUMPIXELS2 20
Adafruit_NeoPixel pixels(NUMPIXELS2, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
}

void loop() {
  for (int i = 0; i < NUMPIXELS1; i++) {
    pixels.setPixelColor(i, pixels.Color(128, 0, 128));
    pixels.show();

  }  for (int i = NUMPIXELS1; i < NUMPIXELS2; i++) {
    pixels.setPixelColor(i, pixels.Color(252, 90, 0));
    pixels.show();
  }
}
