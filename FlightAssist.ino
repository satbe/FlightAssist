#include <HCSR04.h>
#include "FastLED.h"
#define NUM_LEDS 8
#define LED_PIN 8
#define BRIGHTNESS 255 
#include <dmtimer.h>
#include <math.h>
#include <SimpleKalmanFilter.h>
CRGB leds[NUM_LEDS];


// Initialize sensor that uses digital pins 13 and 12.
const byte triggerPin = 3;
const byte echoPin = 4;
const byte ChargePin = 2;
const byte Lipo_Pin = A7;
int Ubatt = 3.4;  
int BattL = 0;
const byte Hmax = 58;    // 70cm New , 58cm Inflateble
int distanceF;

UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);
SimpleKalmanFilter FilterUbatt(2, 2, 0.01);       // Filter to get stable receiver battery level
SimpleKalmanFilter FilterHight(20, 20, 0.8);       // Filter to get stable receiver battery level

void setup () {
    pinMode(ChargePin, INPUT);
    pinMode(Lipo_Pin, INPUT); 
    Serial.begin(57600);  // We initialize serial connection so that we could print values from sensor.
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness( BRIGHTNESS );
    Ubatt = FilterUbatt.updateEstimate(map(analogRead(Lipo_Pin), 0, 1023, 0, 510));
    BattL = (Ubatt/10) - 34;      // 3,4V = 0, 4.2V = 7;
    for(int i=0;i<8;i++){
    Ubatt = FilterUbatt.updateEstimate(map(analogRead(Lipo_Pin), 0, 1023, 0, 510));
    BattL = (Ubatt/10) - 34;      // 3,4V = 0, 4.2V = 7;  
    leds[i] = CRGB(4,0,0);        // Fill  with red, dimmed 
    for(int i=0;i<8;i++){
    leds[i] = CRGB(4,0,0);        // Fill  with red, dimmed 
   }
   for(int i=0;i<BattL;i++){
    leds[i] = CRGB(0,4,0);        // Fill  with green, 3.4-4.2V -> 0-7 green LEDs
   }   
  }
        FastLED.show();
delay(3000);

}

void loop () {  
    int distance = distanceSensor.measureDistanceCm();    
    distanceF = FilterHight.updateEstimate(distance);    //Serial.println(distance);
    //Serial.println(round(distance/10));
    
    LedRing(round(distanceF/(Hmax/7)),(Ubatt = FilterUbatt.updateEstimate(map(analogRead(Lipo_Pin), 0, 1023, 0, 510))/10)-34);    // 0-510mV, /10 -> 34 to 42 = 3.4 to 4.2V -> -34 means 0-8 LEDs
    delay(5);
     // Every 5 miliseconds, do a measurement using the sensor and print the distance in centimeters. 
     Serial.print(distanceF);
     Serial.print(";"); 
}

void LedRing(int dist, int BattLevel) {
  if (dist >= 7) {
    dist = 7;                          // LED 7 ist the last one
  }
  if (dist>-1) {                       // only show something if the measurement is valid
  for(int i=0;i<8;i++){
    leds[i] = CRGB(0,0,64);            // Fill  with blue, dimmed to 25% (256/4=64)
   }
    leds[dist] = CRGB::Green ;
    if (dist == 0) {
    leds[dist] = CRGB(255,0,0);        // Red
    } 
    if (dist == 1) {
    leds[dist] = CRGB(255,128,0);      // Orange
    }       
    if (dist == 6) {
    leds[dist] = CRGB(255,128,0);      // Orange
    }
     if (dist == 7) {
    leds[dist] = CRGB::DarkRed ;       // Red
    }
//--- Show Charge Level when connected to 5V (charging)
 if (digitalRead(ChargePin) == 1 ) {   // when Charging, show battery level (shows charging voltage -> all green
 for(int i=0;i<8;i++){
    leds[i] = CRGB(4,0,0);             // Fill  with red, dimmed 
   }
 if (BattLevel >=8  ){
  BattLevel = 8;
 }
 for(int i=0;i<BattLevel;i++){
    leds[i] = CRGB(0,4,0);             // Fill  with green, 3.4-4.2V -> 0-7 green LEDs
   }   
 }
//--
       FastLED.show();                   
   }
}
