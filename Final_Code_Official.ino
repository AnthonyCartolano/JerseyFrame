#include <IRremote.hpp>
#include "PinDefinitionsAndMore.h"
#include <Adafruit_NeoPixel.h>
#define DEBUG_BUTTON_PIN APPLICATION_PIN
#define PIN 6
#define NUMPIXELS 20
#define NUMPIXELS1 10
#define NUMPIXELS2 20
#define NUMSTRIPS 6

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

int r;
int g;
int b;

void setAllStrips(float red, float green, float blue) {
  uint8_t converted_red;
  uint8_t converted_green;
  uint8_t converted_blue;
  for (int strip = 0; strip < NUMSTRIPS; strip++) {
    for (int pixel = 0; pixel < NUMPIXELS; pixel++) {
      Strips[strip].setPixelColor(pixel, converted_red, converted_green, converted_blue);
    }
    Strips[strip].show();
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
  while (true) {
    for (int i = -1; i < 20; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      r = r - r;
      g = g - g;
      b = b - b;
      delay(100);
      r = 128;
      b = 128;
      pixels.show();
      if (check_ir()) {
        return (0);
      }
    }
    for (int i = -1; i < 20; i++) {
      if (check_ir()) {
        return (0);
      }
      r = r - r;
      g = g - g;
      b = b - b;
      delay(100);
      r = 252;
      g = 90;
      pixels.setPixelColor(i, pixels.Color(252, 90, 0));
      pixels.show();
    }
  }
}

void led_crossfade(float startRed, float startGreen, float startBlue, float endRed, float endGreen, float endBlue) {
  delay(100);
  float Red_Step = (endRed - startRed) / 255;
  float Blue_Step = (endBlue - startBlue) / 255;
  float Green_Step = (endGreen - startGreen) / 255;
  float red = startRed;
  float blue = startBlue;
  float green = startGreen;
  while (true) {
    for (int i = 0; i < 255; i++) {
      if (check_ir() == 1) {
        return (0);
      }
      red = red + Red_Step;
      blue = blue + Blue_Step;
      green = green + Green_Step;
      uint8_t converted_red = round(red);
      uint8_t converted_green = round(green);
      uint8_t converted_blue = round(blue);
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(converted_red, converted_green, converted_blue));
        pixels.show();
      }
      delay(10);
    }
    for (int i = 0; i < 4; i++) {
      if (check_ir() == 1) {
        return (0);
      }
      delay(500);
    }
    for (int i = 0; i < 255; i++) {
      if (check_ir() == 1) {
        return (0);
      }
      red = red - Red_Step;
      blue = blue - Blue_Step;
      green = green - Green_Step;
      uint8_t converted_red = round(red);
      uint8_t converted_green = round(green);
      uint8_t converted_blue = round(blue);
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(converted_red, converted_green, converted_blue));
        pixels.show();
      }
      delay(10);
    }
  }
}

bool check_ir() {
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
        led_crossfade(128, 0, 128, 252, 90, 0);
        break;
      case 0x12:
        led_purple_and_gold();
        break;
      case 0x14:
        led_off();
        led_around();
        break;
    }
    return (true);
  }
  return (false);
}

void setup() {
  pixels.begin();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  check_ir();
}