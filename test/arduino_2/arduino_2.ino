#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)

int pos = 0;

// YunServer server;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// YunClient client;

void setup() {
  //Bridge.begin();           // Let's activate the Yun Bridge...
  pwm.begin();
  pwm.setPWMFreq(60);       // Analog servos run at ~60 Hz updates
  // server.listenOnLocalhost();
  // server.begin();          // Let's acrivate the Yun server...
  pwm.setPWM(0, 0, 150);
  Serial.begin(9600);
}

void loop() {
  int map_nine = map(90,0,180,SERVOMIN,SERVOMAX);

  for (pos = map_nine; pos >= SERVOMIN; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    // third
    pwm.setPWM(3, 0, pos);              // tell servo to go to position in variable 'pos'
    //Serial.print("-");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  Serial.println("3: " + pos);
  delay(500);

  for (pos = map_nine; pos <= SERVOMAX+(SERVOMAX-SERVOMIN); pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    // second.write(pos);              // tell servo to go to position in variable 'pos'
    pwm.setPWM(2, 0, pos);
    //Serial.print("-");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  Serial.println("2: " + pos);
  delay(500);

  for (pos = map_nine; pos >= SERVOMIN; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    //first.write(pos);              // tell servo to go to position in variable 'pos'
    pwm.setPWM(0, 0, pos);
    //Serial.print("-");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  Serial.println("1: "+pos);
  delay(2000);

//----------

  for (pos = SERVOMIN; pos <= map_nine; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    // first.write(pos);              // tell servo to go to position in variable 'pos'
    pwm.setPWM(0, 0, pos);
    //Serial.print("-");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  Serial.println("a: "+pos);
  delay(500);

    for (pos = SERVOMAX+(SERVOMAX-SERVOMIN); pos >= map_nine; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    //second.write(pos);              // tell servo to go to position in variable 'pos'
    pwm.setPWM(2, 0, pos);
    //Serial.print("-");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  Serial.println("1: "+pos);
  delay(500);

  for (pos = SERVOMIN; pos <= map_nine; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    // third.write(pos);              // tell servo to go to position in variable 'pos'
    pwm.setPWM(3, 0, pos);
    //Serial.print("-");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  Serial.println("3: "+pos);
  delay(3000);
}