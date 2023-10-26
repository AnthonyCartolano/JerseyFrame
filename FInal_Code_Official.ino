#include <IRremote.hpp>
#include "PinDefinitionsAndMore.h"
#include <Adafruit_NeoPixel.h>
#define DEBUG_BUTTON_PIN    APPLICATION_PIN
#define PIN 6
#define NUMPIXELS 20
#define NUMPIXELS1 10
#define NUMPIXELS2 20
int r;
int g;
int b;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  pixels.begin();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {

  if (IrReceiver.decode()) {
    IrReceiver.resume();

    //OFF BUTTON
    if (IrReceiver.decodedIRData.command == 0x1) {
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();
      }
    }

    //PURPLE
    if (IrReceiver.decodedIRData.command == 0x10) {
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(128, 0, 128));
        pixels.show();
      }

      //GOLD
    } else if (IrReceiver.decodedIRData.command == 0x11) {
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(252, 90, 0));
        pixels.show();
      }
    }
  }

  //PURPLE AND GOLD
  if (IrReceiver.decodedIRData.command == 0x12) {
    for (int i = 0; i < NUMPIXELS1; i++) {
      pixels.setPixelColor(i, pixels.Color(252, 90, 0));
      pixels.show();
    }  for (int i = NUMPIXELS1; i < NUMPIXELS2; i++) {
      pixels.setPixelColor(i, pixels.Color(128, 0 , 128));
      pixels.show();
    }

    //AROUND
  } else if (IrReceiver.decodedIRData.command == 0x14) {
    for (int i = 0; i < 20; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      r = r - r;
      g = g - g;
      b = b - b;
      delay(100);
      r = 128;
      b = 128;
      pixels.show();
    }  for (int i = 0; i < 20; i++) {
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
}
