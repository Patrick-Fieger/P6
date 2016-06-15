
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)

uint8_t servonum = 0;


void setup() {
  pwm.begin();  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop() {
  
  // slow motion
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
    delay(5);
  }
  delay(1000);

  // instant movement
  pwm.setPWM(0,0,SERVOMIN);

  delay(5000);
}
