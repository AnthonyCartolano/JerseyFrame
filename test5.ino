#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#if FLASHEND >= 0x3FFF
#endif
#if defined(DECODE_BEO)
#define RECORD_GAP_MICROS 16000
#endif
#if !defined(RAW_BUFFER_LENGTH)
#  if RAMEND <= 0x4FF || RAMSIZE < 0x4FF
#define RAW_BUFFER_LENGTH  130
#define EXCLUDE_EXOTIC_PROTOCOLS
#define EXCLUDE_UNIVERSAL_PROTOCOLS
#  elif RAMEND <= 0x8FF || RAMSIZE < 0x8FF
#define RAW_BUFFER_LENGTH  600
#  else
#define RAW_BUFFER_LENGTH  750
#  endif
#endif
#include <IRremote.hpp>
#if defined(APPLICATION_PIN)
#define DEBUG_BUTTON_PIN    APPLICATION_PIN // if low, print timing for each received data set
#else
#define DEBUG_BUTTON_PIN   5
#endif
bool detectLongPress(uint16_t aLongPressDurationMillis);
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
#define NUMPIXELS 20
#define NUMPIXELS1 10
#define NUMPIXELS2 20
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int r;
int g;
int b;

void setup() {
  pixels.begin();
#if FLASHEND >= 0x3FFF
  pinMode(DEBUG_BUTTON_PIN, INPUT_PULLUP);
#endif
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println();
#if FLASHEND >= 0x3FFF
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
      Serial.println(F("Overflow detected"));
      Serial.println(F("Try to increase the \"RAW_BUFFER_LENGTH\" value of " STR(RAW_BUFFER_LENGTH) " in " __FILE__));

#  if !defined(ESP8266) && !defined(NRF5)

#    if !defined(ESP32)
      IrReceiver.stop();
#    endif
      tone(TONE_PIN, 1100, 10);
      delay(50);
      tone(TONE_PIN, 1100, 10);
      delay(50);
#    if !defined(ESP32)
      IrReceiver.start(100000);
#    endif
#  endif

    } else {
      auto tStartMillis = millis();
#    if !defined(ESP32)
      IrReceiver.stop();
#    endif
      tone(TONE_PIN, 2200);


#  if defined(LOCAL_DEBUG)
      IrReceiver.printIRResultShort(&Serial, true);
#  else
      IrReceiver.printIRResultShort(&Serial, true, digitalRead(DEBUG_BUTTON_PIN) == LOW);
#  endif

      while ((millis() - tStartMillis) < 5)
        ;
      noTone(TONE_PIN);

#    if !defined(ESP32)

      IrReceiver.startWithTicksToAdd((millis() - tStartMillis) * (MICROS_IN_ONE_MILLI / MICROS_PER_TICK));
#    endif

      IrReceiver.printIRSendUsage(&Serial);
#  if defined(LOCAL_DEBUG)
      IrReceiver.printIRResultRawFormatted(&Serial, true);
#  else
      if (IrReceiver.decodedIRData.protocol == UNKNOWN || digitalRead(DEBUG_BUTTON_PIN) == LOW) {
        // We have an unknown protocol, print more info
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
          Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
        }
        IrReceiver.printIRResultRawFormatted(&Serial, true);
      }
#  endif
    }

    // tone on esp8266 works once, then it disables the successful IrReceiver.start() / timerConfigForReceive().
#  if !defined(ESP8266) && !defined(NRF5) && !defined(LOCAL_DEBUG)
    if ((IrReceiver.decodedIRData.protocol != SONY) && (IrReceiver.decodedIRData.protocol != PULSE_WIDTH)
        && (IrReceiver.decodedIRData.protocol != PULSE_DISTANCE) && (IrReceiver.decodedIRData.protocol != UNKNOWN)
        && digitalRead(DEBUG_BUTTON_PIN) != LOW) {

#    if !defined(ESP32)
      IrReceiver.stop(); // ESP32 uses another timer for tone()
#    endif
      tone(TONE_PIN, 2200, 8);
#    if !defined(ESP32)
      delay(8);
      IrReceiver.start(8000); // Restore IR timer. 8000 to compensate for 8 ms stop of receiver. This enables a correct gap measurement.
#    endif
    }
#  endif
#else // #if FLASHEND >= 0x3FFF
    // Print a minimal summary of received data
    IrReceiver.printIRResultMinimal(&Serial);
#endif // #if FLASHEND >= 0x3FFF

    IrReceiver.resume();


    // if (IrReceiver.decodedIRData.address == 0) {
    if (IrReceiver.decodedIRData.command == 0x10) {
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(128, 0, 128));
        pixels.show();
      }
    } else if (IrReceiver.decodedIRData.command == 0x11) {
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(252, 90, 0));
        pixels.show();
      }
    }
  }
  if (IrReceiver.decodedIRData.command == 0x12) {
    for (int i = 0; i < NUMPIXELS1; i++) {
      pixels.setPixelColor(i, pixels.Color(252, 90, 0));
      pixels.show();

    }  for (int i = NUMPIXELS1; i < NUMPIXELS2; i++) {
      pixels.setPixelColor(i, pixels.Color(128, 0 , 128));
      pixels.show();
    }
  

} else if (IrReceiver.decodedIRData.command == 0x14) {
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


// Check if the command was repeated for more than 1000 ms
if (detectLongPress(1000)) {
  Serial.print(F("Command 0x"));
  Serial.print(IrReceiver.decodedIRData.command, HEX);
  Serial.println(F(" was repeated for more than 2 seconds"));
}
} // if (IrReceiver.decode())





unsigned long sMillisOfFirstReceive;
bool sLongPressJustDetected;
/**
   @return true once after the repeated command was received for longer than aLongPressDurationMillis milliseconds, false otherwise.
*/
bool detectLongPress(uint16_t aLongPressDurationMillis) {
  if (!sLongPressJustDetected && (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
    /*
       Here the repeat flag is set (which implies, that command is the same as the previous one)
    */
    if (millis() - aLongPressDurationMillis > sMillisOfFirstReceive) {
      sLongPressJustDetected = true; // Long press here
    }
  } else {
    // No repeat here
    sMillisOfFirstReceive = millis();
    sLongPressJustDetected = false;
  }
  return sLongPressJustDetected; // No long press here
}
