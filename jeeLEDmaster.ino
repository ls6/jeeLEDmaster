/// This is a control JeeNode that will instruct a LEDnode to do a nice color transition

#include <JeeLib.h>
#include <FastLED.h>

#define SERIAL_BAUD 57600
#define VERSION 1
#define COLOR_CHANGE_PERIOD 1000
#define TARGET_HUE 36

CRGB leds[1];
MilliTimer timer1;
byte currentHue = 6;

void handleInput (char c) {
  switch (c) {
    case '1':
      Serial.print("got: ");
      Serial.println(c);
      break;
  };
}

static byte newHue () {
  return (currentHue++);
}

void printRGB(byte hue) {
  leds[0] = CHSV(hue, 255, 255);
  Serial.print(hue);
  Serial.print("->");
  Serial.print(leds[0].r);
  Serial.print(" ");
  Serial.print(leds[0].g);
  Serial.print(" ");
  Serial.println(leds[0].b);
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.print("\n[RF12demo.v");
  Serial.print(VERSION);
  Serial.println("]");
}

void loop() {
  if (Serial.available()) {
    handleInput(Serial.read());
  };

  if (currentHue <= TARGET_HUE) {
    if (timer1.poll(COLOR_CHANGE_PERIOD)) {
      printRGB(newHue());
    };
  };
}
