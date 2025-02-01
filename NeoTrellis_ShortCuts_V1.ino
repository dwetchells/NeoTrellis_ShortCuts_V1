/* This example shows basic usage of the NeoTrellis.
  The buttons will light up various colors when pressed.
  The interrupt pin is not used in this example.
  
  https://www.eso.org/~ndelmott/ascii.html Keyboard Codes
  https://github.com/T-vK/ESP32-BLE-Keyboard  BLE Driver

  ATTENTION: BleKeyboard.h downlevel USE ESP32 Libary 2.0.17 to build
  3.0.0+ BLE support does not work
*/

#include "Adafruit_NeoTrellis.h"
#include <BleKeyboard.h>


Adafruit_NeoTrellis trellis;
BleKeyboard bleKeyboard("NeoTrellis_ShortCuts");

/******************************************/

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return trellis.pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return trellis.pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return trellis.pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}
int Action[] = {0x61, 0x63, 0x64, 0x66, 0x67, 0x6c, 0x79, 0x70, 0x71};  // [A, C, D, F]  [G, L, Y, P]  [N, M, ?, ?] [?, ?, ?, A-C-D]

//define a callback for key presses
TrellisCallback blink(keyEvent evt){
  // Check is the pad pressed?
  if(bleKeyboard.isConnected()) {
    digitalWrite(LED_BUILTIN, HIGH); 
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
      trellis.pixels.setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, trellis.pixels.numPixels(), 0, 255))); //on rising
      Serial.println(evt.bit.NUM);
      // Press Keyboard ALT,CTRL, Web Shortcut Key
      bleKeyboard.press(KEY_LEFT_CTRL);
      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press(Action[evt.bit.NUM]);
      delay(100);
      bleKeyboard.releaseAll();
      

     if (evt.bit.NUM == 15){      
      Serial.println("Key 0 has been pressed");
      bleKeyboard.press(KEY_LEFT_CTRL);
      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press(KEY_DELETE);  //LOG OUT
      delay(100);
      bleKeyboard.releaseAll();
      }
      
    } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    // or is the pad released?
      trellis.pixels.setPixelColor(evt.bit.NUM, 0); //off falling
    }
    trellis.pixels.show();
  } else {
    Serial.println("BLE not Connected!");
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000); 
  }
  // Turn on/off the neopixels!
  return 0;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 
  Serial.begin(9600);
  // while(!Serial) delay(1);
  
  if (!trellis.begin()) {
    Serial.println("Could not start trellis, check wiring?");
    while(1) delay(1);
  } else {
    Serial.println("NeoPixel Trellis started");
    Serial.println("Starting BLE work!");
    bleKeyboard.begin();
  }

  //activate all keys and set callbacks
  for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
    trellis.registerCallback(i, blink);
  }
  
  for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, Wheel(map(i, 0, trellis.pixels.numPixels(), 0, 255)));
    trellis.pixels.show();
    delay(50);
  }
  
  for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, 0x000000);
    trellis.pixels.show();
    delay(50);
  }
}

void loop() {
  
    trellis.read();  // interrupt management does all the work! :)
    delay(20); //the trellis has a resolution of around 60hz

}
