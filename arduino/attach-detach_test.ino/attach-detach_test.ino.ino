#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {}
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(3));
  if(myservo.attached() == false && digitalRead(7) == LOW)
  {
    Serial.println("attached");
    myservo.attach(9);
    myservo.write(map(analogRead(A0),425,101,180,0));
  } 
  else if(digitalRead(7) != LOW) {
    Serial.println("detached");
    myservo.detach();
  }
}
