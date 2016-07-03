#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)

int allLEDsPin = 7,
  tableLEDsPin = 8,
  touchPin = 9;

// situations > furnitures > levels > pin, angle
int target_pos[3][3][3][2]={
  { // Stuhl und tisch
   {{1,100},{2,0},{3,180}}, // S
   {{4,135},{5,45},{6,35}}, // T
   {{8,90},{9,175}} // STB
   },
  { // liege
   {{1,160},{2,60},{3,60}}, // S
   {{4,90},{5,90},{6,170}}, // T
   {{8,90},{9,175}} // STB
  },
 { // Gäste
   {{1,10},{2,90},{3,90}}, // S
   {{4,90},{5,90},{6,170}}, // T
   {{8,15},{9,10}} // STB
  }
};

int current_pos[3][3][2],
  initial_pos[3][3][2]={
   {{1,10},{2,90},{3,90}}, // S
   {{4,90},{5,90},{6,170}}, // T
   {{8,90},{9,175}}, // STB
};

int current_state = 0;

bool isRunning = false;
bool isAllFurnitureSet = false;
float difference;
int movementDirection,
  maxAmountLvls = 3;

bool devmode = true;
int how_many_seconds_to_build = 2 * 1000;

YunServer server;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
YunClient client;


void setup() {
//  current_pos = initial_pos;
  memcpy(current_pos,initial_pos,sizeof(initial_pos));
  pwm.begin();
  pwm.setPWMFreq(60);
  Serial.println("Resetting now!");
  resetPositions();

  delay(2000);
  Serial.println("Postioning now!");
  setPositions("2");
  Serial.println("done");

  delay(5000);
  Serial.println("Resetting now!");
  resetPositions();
}

void loop() {

}


/**
 * Funktion zum ausführen der Bewegungen um einen Zustand zu erreichen
 */
void setPositions(String situationID_string){
	int situationID = situationID_string.toInt();
	isRunning = true;
  for(int currentLvl = maxAmountLvls-1;currentLvl >= 0;currentLvl--) // each level
  {
    do { // until all furniture is set in this level
      isAllFurnitureSet = true; // nur ne Behauptung zum widerlegen
      for(int piece=0;piece<3;piece++) // each furniture
      {
        if(target_pos[situationID][piece][0][0] != 0)
        {
          difference = target_pos[situationID][piece][currentLvl][1] - current_pos[piece][currentLvl][1];
          if(difference != 0)
          {
            isAllFurnitureSet = false;
            movementDirection = difference/abs(difference);
            // move one step closer to target
            current_pos[piece][currentLvl][1] += movementDirection;
              //Serial.println(String(current_pos[piece][currentLvl][0]) + " => " + current_pos[piece][currentLvl][1] + "(" + difference + ")");

            pwm.setPWM(current_pos[piece][currentLvl][0],0,map(current_pos[piece][currentLvl][1],0,180,SERVOMIN,SERVOMAX));

          }
        }
      }
      delay(15);
    } while(isAllFurnitureSet == false);
  }

	isRunning = false;
}

void resetPositions() {
  Serial.println("RESETTING");

  isRunning = true;
  for(int currentLvl = 0;currentLvl < maxAmountLvls;currentLvl++)
  {
    do { // until all furniture is set in this level
      isAllFurnitureSet = true; // nur ne Behauptung zum widerlegen
      for(int piece=0;piece<3;piece++) // for each furniture
      {
          difference = initial_pos[piece][currentLvl][1] - current_pos[piece][currentLvl][1];
          if(difference != 0)
          {
            isAllFurnitureSet = false;
            movementDirection = difference/abs(difference);
            // move one step closer to target
            current_pos[piece][currentLvl][1] += movementDirection;
            Serial.println(String(current_pos[piece][currentLvl][0]) + " => " + current_pos[piece][currentLvl][1] + "(" + difference + ")");

            pwm.setPWM(current_pos[piece][currentLvl][0],0,map(current_pos[piece][currentLvl][1],0,180,SERVOMIN,SERVOMAX));
          }
      }
      delay(15);
    } while(isAllFurnitureSet == false);
  }

  isRunning = false;
}

// Initialize Edit Mode
void initEditmode() {
  //turn on all LEDs
  digitalWrite(allLEDsPin, HIGH);
  digitalWrite(tableLEDsPin, HIGH);
  
  while(touchPin != HIGH) {
    delay(5);
  }
  startEditmode();
}

void startEditmode() {
  digitalWrite(allLEDsPin, LOW); // turn of other LEDs than table
  for(n=0;n<maxAmountLvls;n++) 
  {
    pwm.setPWM(initial_pos[1][n][0],0,0); // free all table servos 
  }

  while(touchPin == HIGH) {
    delay(5);
  }
  endEditmode();
}

void endEditmode() {
  digitalWrite(tableLEDsPin, LOW);
  for(n=0;n<maxAmountLvls;n++) 
  {
    target_pos[0][1][n][1] = map(analogRead(readPins[n]),READMIN,READMAX,0,180) // update situation data
    pwm.setPWM(initial_pos[1][n][0],0,map(target_pos[0][1][n][1],0,180,SERVOMIN,SERVOMAX)); // free all table servos 
  }    
}

