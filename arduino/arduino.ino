#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650 // this is the 'maximum' pulse length count (out of 4096)


// situations > furnitures > levels > pin, angle
int target_pos[3][2][3][2]={
  { // Stuhl und tisch
   {{3,0},{2,180},{0,0}},
   {{0,90},{1,90}}
  },
  { // liege
    {{0,120},{1,10}}
  },
 { // Gäste
    {{0,180},{1,180}}
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
  maxAmountLvls = 3;

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
    isAllFurnitureSet = true; // nur ne Behauptung zum widerlegen
    do { // until all furniture is set in this level
      for(int piece=0;piece<sizeof(target_pos[situationID]);piece++) // each furniture
      {
        Serial.println(sizeof(target_pos[situationID][piece]));
        if(sizeof(target_pos[situationID][piece]) > currentLvl)
        {
          difference = target_pos[situationID][piece][currentLvl][1] - current_pos[situationID][piece][currentLvl][1];
          if(difference != 0)
          {
            isAllFurnitureSet = false;
            movementDirection = difference/abs(difference);
            // move one step closer to target
            current_pos[situationID][piece][currentLvl][1] += movementDirection;
            //if(piece == 0 && currentLvl == 0)
            //  Serial.println(String(current_pos[situationID][piece][currentLvl][0]) + " => " + current_pos[situationID][piece][currentLvl][1] + "(" + difference + ")");
          }
        }
      }
      delay(150);
    } while(isAllFurnitureSet == false);
  }

	isRunning = false;
}



