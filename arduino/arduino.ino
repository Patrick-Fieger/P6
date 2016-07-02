#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)


// situations > furnitures > levels > pin, angle
int target_pos[3][3][3][2]={
  { // Stuhl und tisch
   {{1,180},{2,100},{3,90}}, // S
   {{4,180},{5,150},{6,170}}, // T
   {{8,0},{9,3}}, // STB
  },
  { // liege
   {{1,10},{2,90},{3,90}}, // S
   {{4,90},{5,90},{6,170}}, // T
   {{8,90},{9,100}}, // STB
  },
 { // Gäste
   {{1,10},{2,90},{3,90}}, // S
   {{4,90},{5,90},{6,170}}, // T
   {{8,90},{9,100}}, // STB
  },
};

int current_pos[3][3][2],
  initial_pos[3][3][2]={
   {{1,10},{2,90},{3,90}}, // S
   {{4,90},{5,90},{6,170}}, // T
   {{8,90},{9,100}}, // STB
};

int current_state = 0;

bool isRunning = false;
bool isAllFurnitureSet = false;
float difference;
int movementDirection,
  maxAmountLvls = 3;

bool devmode = true;
int how_many_seconds_to_build = 2 * 1000;

void setup() {
//  current_pos = initial_pos;
  memcpy(current_pos,initial_pos,sizeof(initial_pos));

  setPositions("0");
  Serial.println("done");
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
  for(int currentLvl = 0;currentLvl < maxAmountLvls;currentLvl++) // each level
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
              Serial.println(String(current_pos[piece][currentLvl][0]) + " => " + current_pos[piece][currentLvl][1] + "(" + difference + ")");
          }
        }
      }
      delay(5);
    } while(isAllFurnitureSet == false);
  }

	isRunning = false;
}

void resetPositions() {
  isRunning = true; 
  for(int currentLvl = maxAmountLvls-1;currentLvl >= 0;currentLvl--) // each level
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
          }
      }
      delay(5);
    } while(isAllFurnitureSet == false);
  }

  isRunning = false;
}



