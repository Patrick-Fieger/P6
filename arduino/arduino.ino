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
int how_many_seconds_to_build = 5 * 1000;
bool go = true;
bool devmode = true;


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


	if(go){
    //edit();

		client = server.accept();

		if (client){
		  String command = client.readString();
		  command.trim();

		  if(command == "0"){
		  	edit();
		  }else{
		    setPositions(command);
		  }

		  client.print(command);
		  client.stop();
		}
	}
	delay(50);
}

void setPositions(String number){
  go = false;
  int num = number.toInt();
  for(int i = 0; i < maxItemsInArray; i++){
  	  int deg = pos[num][i][1];
  	  int last_pos_map = map(lastpos[i],0,180,SERVOMIN,SERVOMAX);
  	  int pwm_num = pos[num][i][0];
  	  int deg_map = map(pos[num][i][1],0,180,SERVOMIN,SERVOMAX);


      if(lastpos[i] > deg){
      	s("letzte Position ist GRÖSSER als die die eingestellt werden soll");
      	int delay_calc = calcDelay(last_pos_map,deg_map);

      	for (int k = last_pos_map; k > deg_map; k--) {
            pwm.setPWM(pwm_num, 0, k);
            delay(delay_calc);
        }
      }else if(lastpos[i] < deg){
      	s("letzte Position ist KLEINER als die die eingestellt werden soll");
      	int delay_calc = calcDelay(deg_map,last_pos_map);

      	for (int j = last_pos_map; j < deg_map; j++) {
            pwm.setPWM(pwm_num, 0, j);
            delay(delay_calc);
        }

      }else{
      	s("letzte Position ist GENAUSO GROSS als die die eingestellt werden soll");
      }

      lastpos[i] = deg;
  }

  go = true;
}

float calcDelay(int a, int b){
	return how_many_seconds_to_build / (a-b);
}

void s(String ausgabe){
	if(devmode) Serial.println(ausgabe);

};

void setPWMWith(int servo, int angle){
  pwm.setPWM(servo, 0, map(angle,121,506,SERVOMIN,SERVOMAX));
  //pwm.setPWM(servo, 0, map(angle,150,650,SERVOMIN,SERVOMAX));
}

void edit(){
  go = false;
  delay(1000);
	int analogPin = 0;
	int val = 0;
	val = analogRead(analogPin);

  // int start[15] = {120,130,180,210,240,270,300,330,360,390,420,450,480,510};


  // for(int i=0; i< sizeof(start); i++){
  //   if(val > start[i] && val < start[i+1]){
  //     setPWMWith(0,start[i]+10);
  //   }
  // }

  // s(String(map(val,121,506,SERVOMIN,SERVOMAX)));

  if(val > 120 && val < 140){
    setPWMWith(0,130);
  }else if(val > 140 && val < 160){
    s("hieeeerr!!");
    setPWMWith(0,150);
  }else if(val > 160 && val < 180){
    setPWMWith(0,170);
  }else if(val > 180 && val < 200){
    setPWMWith(0,190);
  }else if(val > 200 && val < 220){
    setPWMWith(0,210);
  }else if(val > 220 && val < 240){
    setPWMWith(0,230);
  }

  else if(val > 240 && val < 260){
    setPWMWith(0,250);
  }

  else if(val > 260 && val < 280){
    setPWMWith(0,270);
  }

  else if(val > 280 && val < 300){
    setPWMWith(0,290);
  }

  else if(val > 300 && val < 320){
    setPWMWith(0,310);
  }


  else if(val > 320 && val < 340){
    setPWMWith(0,340);
  }

  else if(val > 340 && val < 360){
    setPWMWith(0,350);
  }

  else if(val > 360 && val < 380){
    setPWMWith(0,370);
  }

  else if(val > 380 && val < 400){
    setPWMWith(0,390);
  }

  // }else if(val > 300 && val < 330){
  //   setPWMWith(0,310);
  // }else if(val > 330 && val < 360){
  //   setPWMWith(0,340);
  // }else if(val > 360 && val < 390){
  //   setPWMWith(0,370);
  // }else if(val > 390 && val < 420){
  //   setPWMWith(0,400);
  // }else if(val > 420 && val < 450){
  //   setPWMWith(0,430);
  // }else if(val > 450 && val < 480){
  //   setPWMWith(0,460);
  // }else if(val > 480){
  //   setPWMWith(0,500);
  // }

  // 121,506

  // if()

	// pwm.setPWM(0, 0, map(val,150,650,SERVOMIN,SERVOMAX));
 //

    // pwm.setPWM(0, 0, map(val,121,506,SERVOMIN,SERVOMAX));
    s(String(map(val,121,506,SERVOMIN,SERVOMAX)));

    go = true;
}
