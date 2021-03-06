// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Wire.h>

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  
//  Serial.begin(9600);           // start serial for output/debug
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("--Start--");
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()

void receiveEvent(int howMany) {
  Serial.print("Peek len: ");         // print the integer
  Serial.println(howMany);         // print the integer

  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

void requestEvent() {
  Serial.println("req_event");
  
  Serial.print("xtesting: ");         // print the integer
  int x=Wire.available();
  Serial.println(x);

  Wire.write("hello "); // respond with message of 6 bytes
  
  // as expected by master
}



