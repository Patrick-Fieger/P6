#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)


// situations > furnitures > levels > pin, angle
int target_pos[3][2][3][2]={
  { // Stuhl und tisch
   {{3,1},{2,180},{1,1}},
   {{1,90},{2,90}}
  },
  { // liege
    {{1,120},{2,10}}
  },
 { // Gäste
    {{1,180},{2,180}}
 }
};

int current_pos[3][2][3][2],
 initial_pos[3][2][3][2]={
  { // Stuhl und tisch
   {{3,90},{2,90},{0,90}},
   {{0,90},{1,90}}
  },
  { // liege
    {{0,90},{1,90}}
  },
 { // Gäste
    {{0,90},{1,90}}
 }
};

int current_state = 0;

bool isRunning = false;
bool isAllFurnitureSet = false;
float difference;
int movementDirection,
  maxAmountLvls = 3,
  maxAmountOfPieces = 2;

bool devmode = true;
int how_many_seconds_to_build = 2 * 1000;

void setup() {
//  current_pos = initial_pos;
  memcpy(current_pos,initial_pos,sizeof(initial_pos));

  setPositions("2");
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
      for(int piece=0;piece<maxAmountOfPieces;piece++) // each furniture
      {
        if(target_pos[situationID][piece][0][0] != 0)
        {
          difference = target_pos[situationID][piece][currentLvl][1] - current_pos[situationID][piece][currentLvl][1];
          if(difference != 0)
          {
            isAllFurnitureSet = false;
            movementDirection = difference/abs(difference);
            // move one step closer to target
            current_pos[situationID][piece][currentLvl][1] += movementDirection;
            Serial.println(String(current_pos[situationID][piece][currentLvl][0]) + " => " + current_pos[situationID][piece][currentLvl][1] + "(" + difference + ")");
          }
        }
      }
      delay(5);
    } while(isAllFurnitureSet == false);
  }

	isRunning = false;
}



