/// This is a control JeeNode that will instruct a LEDnode to do a nice color transition

#include <JeeLib.h>
#include <FastLED.h>

#define SERIAL_BAUD 57600
#define VERSION 1

CRGB leds[1];

static void handleInput (char c) {
  switch (c) {
    case '1':
      Serial.print("got: ");
      Serial.println(c);
      leds[0] = CHSV(224, 187, 255);
      Serial.print("red: ");
      Serial.println(leds[0].r);
      Serial.print("green: ");
      Serial.println(leds[0].g);
      Serial.print("blue: ");
      Serial.println(leds[0].b);
      break;
  };
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.print("\n[RF12demo.v");
  Serial.print(VERSION);
  Serial.println("]");
}

void loop() {
  if (Serial.available())
  handleInput(Serial.read());
}
