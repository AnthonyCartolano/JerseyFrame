#include <IRremote.hpp>
#include "PinDefinitionsAndMore.h"
#include <Adafruit_NeoPixel.h>
#define DEBUG_BUTTON_PIN APPLICATION_PIN
#define PIN 6
#define NUMPIXELS 20
#define NUMPIXELS1 10
#define NUMPIXELS2 20
#define NUMSTRIPS 6

int r;
int g;
int b;

const int Pins[NUMSTRIPS] = { 2, 3, 4, 5, 6, 7 };
bool looping_fade = false;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel Strips[NUMSTRIPS] = {
  Adafruit_NeoPixel(NUMPIXELS, Pins[0], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMPIXELS, Pins[1], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMPIXELS, Pins[2], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMPIXELS, Pins[3], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMPIXELS, Pins[4], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMPIXELS, Pins[5], NEO_GRB + NEO_KHZ800)
};

const uint32_t GOLD = Adafruit_NeoPixel::Color(252, 90, 0);
const uint32_t PURPLE = Adafruit_NeoPixel::Color(128, 0, 128);

void setAllStrips(byte red, byte green, byte blue) {
  // for each strip
  for (int strip = 0; strip < NUMSTRIPS; strip++) {
    // for each pixel
    for (int pixel = 0; pixel < NUMPIXELS; pixel++) {
      Strips[strip].setPixelColor(pixel, red, green, blue);
    }
    Strips[strip].show();  // Output to strip
  }
}

void led_crossfade(const uint32_t startColor, const uint32_t endColor, unsigned long speed) {
  byte startRed = (startColor >> 16) & 0xff;
  byte startGreen = (startColor >> 8) & 0xff;
  byte startBlue = startColor & 0xff;

  byte endRed = (endColor >> 16) & 0xff;
  byte endGreen = (endColor >> 8) & 0xff;
  byte endBlue = endColor & 0xff;
  for (int step = 0; step < 256; step++) {
    delay(speed);
    byte red = map(step, 0, 255, startRed, endRed);
    byte green = map(step, 0, 255, startGreen, endGreen);
    byte blue = map(step, 0, 255, startBlue, endBlue);
    setAllStrips(red, green, blue);
  }
}

void led_off() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}
void led_purple() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(128, 0, 128));
    pixels.show();
  }
}

void led_gold() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(252, 90, 0));
    pixels.show();
    looping_fade = false;
  }
}

void led_purple_and_gold() {
  for (int i = 0; i < NUMPIXELS1; i++) {
    pixels.setPixelColor(i, pixels.Color(252, 90, 0));
    pixels.show();
  }
  for (int i = NUMPIXELS1; i < NUMPIXELS2; i++) {
    pixels.setPixelColor(i, pixels.Color(128, 0, 128));
    pixels.show();
  }
}

void led_around() {
  for (int i = 0; i < 20; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    r = r - r;
    g = g - g;
    b = b - b;
    delay(100);
    r = 128;
    b = 128;
    pixels.show();
  }
  for (int i = 0; i < 20; i++) {
    r = r - r;
    g = g - g;
    b = b - b;
    delay(100);
    r = 252;
    g = 90;
    pixels.show();
    pixels.setPixelColor(i, pixels.Color(252, 90, 0));
    pixels.show();
  }
}


void setup() {
  pixels.begin();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    switch (IrReceiver.decodedIRData.command) {
      case 0x1:
        led_off();
        break;
      case 0x10:
        led_purple();
        break;
      case 0x11:
        led_gold();
        break;
      case 0x15:
        led_crossfade(GOLD, PURPLE, 10);
        led_crossfade(PURPLE, GOLD, 10);
        led_crossfade() break;
      case 0x12:
        led_purple_and_gold();
        break;
      case 0x14:
        led_around();
        break;
    }
  }
}
