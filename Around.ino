#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
#define NUMPIXELS1 10
#define NUMPIXELS2 20
Adafruit_NeoPixel pixels(NUMPIXELS2, PIN, NEO_GRB + NEO_KHZ800);

int r;
int g;
int b;
void setup() {
  pixels.begin();
}

void loop() {
  for (int i = 0; i < 20; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    r = r - r;
    g = g - g;
    b = b - b;
    delay(200);
    r = 128;
    b = 128;
    pixels.show();

  }  for (int i = 0; i < 20; i++) {
    r = r - r;
    g = g - g;
    b = b - b;
    delay(200);
    r = 252;
    g = 90;
    pixels.show();
    pixels.setPixelColor(i, pixels.Color(252, 90, 0));
    pixels.show();
  }
}
