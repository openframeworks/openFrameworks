/**
 * openFrameworks Serial Test for Arduino
 * 
 */

#include "Arduino.h"

int ledPin = 13;   // select the pin for the LED
int val = 0;       // variable to store the data from the serial port

void setup() {
  pinMode(ledPin,OUTPUT);   // declare the LED's pin as output
  Serial.begin(9600);        // connect to the serial port
}

void loop () {
  // read the serial port
  val = Serial.read();

  // if the input is '-1' then there is no data
  // at the input, otherwise check out if it is 'a'
  // if it is, send back a message "ABC"
  // and turn on and off the LED on pin 13
  
  
  if (val != -1) {
    if (val == 'a') {
      Serial.print("ABC");        // send back a message simple as ABC
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
    }
  }
}