#include <ESP32Servo.h>
#include "Adafruit_VL53L0X.h"


Adafruit_VL53L0X lox = Adafruit_VL53L0X();

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
int timeDelay = 10;
int angle = 120;  // angle it twists to
int new_angle = 0;

bool currentOpen = false;

int minDistance = 1000;

int reading = 0;

void setup() {
  Serial.begin(115200);

  //  servo setup:
  myservo.attach(15);  // attaches the servo on pin 9 to the servo object
  myservo.writeMicroseconds(1500);
  myservo.write(0); 
  pinMode(LED_BUILTIN, OUTPUT);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  //  sensor setup:
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("cultivating communities lotti"));   
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  reading = measure.RangeMilliMeter;

  if (int(reading) < int(minDistance)){
    if (currentOpen == false) {
      Serial.println("Trigger Open");
      FlowerOpen(angle);
    }
  } else if (int(reading) > int(minDistance)) {
    if (currentOpen == true) {
      Serial.println("Trigger Close");
      FlowerClose(angle);
    }
  } else {
    Serial.println("We fuckd up!");
  }
  Serial.print("Distance: ");
  Serial.println(measure.RangeMilliMeter);
  delay(100);

  //Serial.println("Main Loop");
  //Serial.println(currentOpen);
}

void FlowerOpen(int targetAngle) {
  Serial.println("Opening");
  new_angle = map(targetAngle,0,90,0,116);  
  for (pos = 0; pos <= new_angle; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(timeDelay);                       // waits 15ms for the servo to reach the position
  }
  currentOpen = true;
  Serial.println("End of flower open");
  Serial.print("Open?: ");
  Serial.println(currentOpen);
}


void FlowerClose(int targetAngle){
  Serial.println("Closing");
  new_angle = map(targetAngle,0,90,0,116);
  
  for (pos = new_angle; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
   // Serial.print("Current Pos: ");
    //Serial.println(pos);
    delay(timeDelay);                       // waits 15ms for the servo to reach the position
  }
  currentOpen = false;
  Serial.println("End of flower close");
  Serial.print("Open?: ");
  Serial.println(currentOpen);
  
}
