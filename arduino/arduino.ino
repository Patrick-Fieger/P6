#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)

int maxItemsInArray = 2;
int pos[4][2][2]={
 {{0,0},{1,0}},
 {{0,0},{1,0}},
 {{0,90},{1,90}},
 {{0,180},{1,180}}
};

int  lastpos[2] = {0,0};



YunServer server;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
YunClient client;

void setup() {
  Bridge.begin();           // Let's activate the Yun Bridge...
  pwm.begin();
  pwm.setPWMFreq(60);       // Analog servos run at ~60 Hz updates
  server.listenOnLocalhost();
  server.begin();          // Let's acrivate the Yun server...
  pwm.setPWM(0, 0, 150);
}

void loop() {
  client = server.accept();

  if (client){
    String command = client.readString();
    command.trim();

    if(command == "0"){
      edit();
    }else{
      Serial.println("SETPOSITION!!!");
      setPositions(command);
    }

    client.print(command);
    client.stop();
  }

  delay(50);
}

void setPositions(String number){
  int num = number.toInt();
  for(int i = 0; i < maxItemsInArray; i++){

  	  //int last_pos_map = map(lastpos[i],0,180,SERVOMIN,SERVOMAX);


      Serial.println("-------------------");
      //Serial.println(last_pos_map);
      //Serial.println(deg_map);

      pwm.setPWM(pos[num][i][0], 0, map(pos[num][i][1],0,180,SERVOMIN,SERVOMAX));



      // HIER TIM FOR-SCHLEIFEN

      // if(lastpos[i] > deg){
      // 	Serial.println("letzte Position ist GRÖSSER als die die eingestellt werden soll");

      // 	for (uint16_t k = last_pos_map; last_pos_map > deg_map; k--) {
      //       pwm.setPWM(pwm_num, 0, k);
      //       delay(5);
      //   }

      //   delay(2000);
      // }else if(lastpos[i] < deg){
      // 	Serial.println("letzte Position ist KLEINER als die die eingestellt werden soll");
      // 	for (uint16_t j = last_pos_map; last_pos_map < deg_map; j++) {
      //       pwm.setPWM(pwm_num, 0, j);
      //       delay(5);
      //   }

      //   delay(2000);
      // }else{
      // 	Serial.println("letzte Position ist GENAUSO GROSS als die die eingestellt werden soll");
      // }

      //lastpos[i] = deg;
  }
}

void edit(){

}