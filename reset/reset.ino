#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);       // Analog servos run at ~60 Hz updates
  pwm.setPWM(0, 0, map(0,0,180,SERVOMIN,SERVOMAX));
}

void loop() {
	//edit();



}