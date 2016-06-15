#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)

int maxItemsInArray = 3;
int pos[3][2][2]={
 {{0,0},{1,0}},
 {{0,90},{1,90}},
 {{0,180},{1,180}},
};

int lastpos[2] = {0,0};

YunServer server;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Bridge.begin();           // Let's activate the Yun Bridge...
  pwm.begin();
  pwm.setPWMFreq(60);       // Analog servos run at ~60 Hz updates
  server.begin();          // Let's acrivate the Yun server...
}

void loop() {
  YunClient client = server.accept();

  if (client){
    String command = client.readString();
    command.trim();
    if(command == "0"){
      edit();
    }else{
      setPositions(command);
    }

    client.stop();
  }
}

void setPositions(String number){
  int num = number.toInt();

  for(int i=0; i < maxItemsInArray; i++){
      int deg = map(pos[num][i][1],0,180,SERVOMIN,SERVOMAX);

      if(lastpos[i] > deg){
        for (int pulselen = lastpos[i]; lastpos[i] > deg; pulselen--) {
            pwm.setPWM(pos[num][i][0], 0, pulselen);
            delay(10);
        }
      }else{
        for (int pulselen = lastpos[i]; lastpos[i] < deg; pulselen++) {
            pwm.setPWM(pos[num][i][0], 0, pulselen);
            delay(10);
        }
      }

      lastpos[i] = pos[num][i][1];
  }
}

void edit(){

}