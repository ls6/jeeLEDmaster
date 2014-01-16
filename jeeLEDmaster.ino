/// This is a control JeeNode that will instruct a LEDnode to do a nice color transition

#include <JeeLib.h>
#include <FastLED.h>

#define SERIAL_BAUD 57600
#define VERSION 1
#define THIS_ADDRESS 6
#define TARGET_ADDRESS 1
#define COLOR_CHANGE_PERIOD 1000
#define TARGET_HUE 6

CRGB leds[1];
MilliTimer timer1, sendTimer;
byte currentHue = 36;
byte pendingOutput;
typedef struct {
  byte preset;
  byte r;
  byte g;
  byte b;
  byte time;
  byte next;
} Payload;
Payload outData;


void handleInput (char c) {
  switch (c) {
    case '1':
      Serial.print("got: ");
      Serial.println(c);
      currentHue = 36;
      break;
  };
}

static byte newHue () {
  return (currentHue--);
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

static byte produceOutData () {
  leds[0] = CHSV(currentHue, 255, 255);
  outData.preset = 0;
  outData.r = leds[0].r;
  outData.g = leds[0].g;
  outData.b = leds[0].b;
  outData.time = 0;
  outData.next = 0;
  return 1;
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.print("\n[RF12demo.v");
  Serial.print(VERSION);
  Serial.println("]");
  rf12_initialize(THIS_ADDRESS, RF12_868MHZ, 19);
}

void loop() {
  //reading serial commands
  if (Serial.available()) {
    handleInput(Serial.read());
  };

  //changing colors
  if (currentHue >= TARGET_HUE) {
    if (timer1.poll(COLOR_CHANGE_PERIOD)) {
      newHue();
      pendingOutput = produceOutData();
      //printRGB(newHue());
    };
  };

  /////////////////////////////
  //sending data over the air//
  /////////////////////////////
  
  //this one gets the radio running
  //but we ignore incoming packets
  rf12_recvDone();

  //send when possible
  if (pendingOutput && rf12_canSend()) {
    rf12_sendStart (RF12_HDR_DST | TARGET_ADDRESS, &outData, sizeof outData);
    Serial.print("Sending data: ");
    Serial.print(outData.r);
    Serial.print(" ");
    Serial.print(outData.g);
    Serial.print(" ");
    Serial.println(outData.b);
    rf12_sendWait(0);
    pendingOutput = 0;
  }
}
