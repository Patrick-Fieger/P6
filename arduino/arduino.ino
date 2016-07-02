#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)


int pos[3][3][2]={
 {{3,0},{2,180},{0,0}}, // Stuhl und tisch
 {{0,90},{1,90}}, // Stuhl
 {{0,180},{1,180}} // Liege
};


int reset_array[3][3][2]={
 {{3,60},{2,90},{0,60}}, // Tisch mit Stuhl
 {{10,90},{10,90}}, // Stuhl
 {{10,180},{10,180}} // Liege
};

int reset_array_smooth[3][3][2]={
 {{0,60},{2,90},{3,60}}, // Tisch mit Stuhl
 {{10,90},{10,90}}, // Stuhl
 {{10,180},{10,180}} // Liege
};

int rows[3] = {3,2,2};

int current_state = 0;

bool go = true;
bool devmode = true;
int how_many_seconds_to_build = 2 * 1000;


YunServer server;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
YunClient client;

void setup() {
	Bridge.begin();
	pwm.begin();
	pwm.setPWMFreq(60);
	Serial.begin(9600);
	server.begin();
	resetAllPWMs();
}

void loop() {


	pwm.setPWM(0, 0, map(60,0,180,SERVOMIN,SERVOMAX));

	// if(go){
	// 	client = server.accept();

	// 	if (client){
	// 		String command = client.readString();
	// 		command.trim();

	// 		client.print(command);
	// 		client.stop();

	// 		if(command == "bearbeiten"){
	// 			edit(); // Bearbeiten-Modus
	// 		}else{
	// 			setPositions(command); // Setzen der Servos auf einen bestimmten Zustand
	// 		}

	// 	}
	// }
	// delay(50);
}


/**
 * Funktion zum ausführen der Bewegungen um einen Zustand zu erreichen
 */
int durchlauf = 0;
void setPositions(String number){
	go = false;
	if(durchlauf > 0) resetPWMs();

	durchlauf++;

	int current_state = number.toInt();
	int num = number.toInt();
	for(int i = 0; i < rows[num]; i++){
			int deg = pos[num][i][1];
			int pwm_num = pos[num][i][0];
			int ninety_deg = map(reset_array[num][i][1],0,180,SERVOMIN,SERVOMAX);
			int deg_map = map(pos[num][i][1],0,180,SERVOMIN,SERVOMAX);
			int delay_calc = calcDelay(deg_map,150);

			if(ninety_deg > deg_map){
				s("letzte Position ist GRÖSSER als die die eingestellt werden soll");
				int delay_calc = calcDelay(ninety_deg,deg_map);

				for (int k = ninety_deg; k > deg_map; k--) {
					pwm.setPWM(pwm_num, 0, k);
					delay(5);
				}
			}else if(ninety_deg < deg_map){
				s("letzte Position ist KLEINER als die die eingestellt werden soll");
				int delay_calc = calcDelay(deg_map,ninety_deg);

				for (int j = ninety_deg; j < deg_map; j++) {
					pwm.setPWM(pwm_num, 0, j);
					delay(5);
				}

			}else{
				s("letzte Position ist GENAUSO GROSS als die die eingestellt werden soll");
			}
	}

	go = true;
}

/**
 * Funktion zum zürecksetzen des aktuellen Standes --> wieder in die Ausgangsposition zurück
 */

void resetPWMs(){

	for(int i = 0; i < rows[current_state]; i++){
		int deg = reset_array_smooth[current_state][i][1];
		int pwm_num = reset_array_smooth[current_state][i][0];
		int ninety_deg = map(pos[current_state][i][1],0,180,SERVOMIN,SERVOMAX);
		int deg_map = map(reset_array[current_state][i][1],0,180,SERVOMIN,SERVOMAX);
		int delay_calc = calcDelay(deg_map,150);

		if(ninety_deg > deg_map){
			s("letzte Position ist GRÖSSER als die die eingestellt werden soll");
			int delay_calc = calcDelay(ninety_deg,deg_map);

			for (int k = ninety_deg; k > deg_map; k--) {
				pwm.setPWM(pwm_num, 0, k);
				delay(5);
			}
		}else if(ninety_deg < deg_map){
			s("letzte Position ist KLEINER als die die eingestellt werden soll");
			int delay_calc = calcDelay(deg_map,ninety_deg);

			for (int j = ninety_deg; j < deg_map; j++) {
				pwm.setPWM(pwm_num, 0, j);
				delay(5);
			}

		}else{
			s("letzte Position ist GENAUSO GROSS als die die eingestellt werden soll");
		}
	}

	delay(3000);
}

/**
 * Funktion zum initialen zurücksetzen der Servo-Motoren
 */
void resetAllPWMs(){
	for(int k=0; k < 3; k++){
		for(int i=0; i< rows[k]; i++){
			pwm.setPWM(reset_array_smooth[k][i][0], 0, map(reset_array_smooth[k][i][1],0,180,SERVOMIN,SERVOMAX));
		}
	}
}


/**
 * Funktion berechnet die Zeit die ein Servo benötigen muss um den Zustand in n Zeit zu erreichen
 */
float calcDelay(int a, int b){
	return how_many_seconds_to_build / (a-b);
}


/**
 * Helper-Function zur Ausagebe auf der Konsole
 */
void s(String ausgabe){
	if(devmode) Serial.println(ausgabe);

};

/**
 *  Funktion zum Setzen der Servos in der Bearbeitung
 */
void setPWMWith(int servo, int angle){
	pwm.setPWM(servo, 0, map(angle,121,506,SERVOMIN,SERVOMAX));
}

/**
 * Führt die Bearbeiten Funktion aus
 */
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
	}else if(val > 300 && val < 330){
	  setPWMWith(0,310);
	}else if(val > 330 && val < 360){
	  setPWMWith(0,340);
	}else if(val > 360 && val < 390){
	  setPWMWith(0,370);
	}else if(val > 390 && val < 420){
	  setPWMWith(0,400);
	}else if(val > 420 && val < 450){
	  setPWMWith(0,430);
	}else if(val > 450 && val < 480){
	  setPWMWith(0,460);
	}else if(val > 480){
	  setPWMWith(0,500);
	}

	pwm.setPWM(0, 0, map(val,121,506,SERVOMIN,SERVOMAX));
	s(String(map(val,121,506,SERVOMIN,SERVOMAX)));

	go = true;
}
