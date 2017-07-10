#include <Asuro.h>

#define KP 0.2
#define BASE 100
#define TICKS_ANGLE 30
#define ODOTHRESHOLD 80
#define MAX_BARCODE_DISTANCE 10
#define DARK 100
#define WHITE 800
Asuro asuro = Asuro();

unsigned int l_data[2];
unsigned int speed[2];

unsigned int odo_data[2];
unsigned int old_odo_data[2];
unsigned int ticks[2];

unsigned int ticks_since_last_dark;
unsigned int barcode_count;
enum BARCODE_STATE {
  BRIGHT,
  BLACK,
  STOP
};
BARCODE_STATE b_state;
enum ASURO_STATE{
  FIND_LINE,
  SEARCH_LINE,
  FOLLOW_LINE,
  SCAN_BARCODE,
  BLINK,
  STOP_ASURO
};
ASURO_STATE a_state;
void findTick(int id){
  int diff = odo_data[id] - old_odo_data[id];
  diff = abs(diff);
  if (diff > ODOTHRESHOLD ){
    // tick found
    ticks[id]++;
  }
  old_odo_data[id] = odo_data[id];
}

void turnRight(){
  asuro.readOdometry(old_odo_data);
  // asuro on the move
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(BASE,0);
  if (l_data[0] < DARK){
  //  a_state = FOLLOW_LINE;
    asuro.setMotorSpeed(0,0);
    return;
  }
      asuro.readOdometry(odo_data);
      findTick(0);
      Serial.print(ticks[0]);
      if(ticks[0] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving

        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
          return;
      }

}
void turnLeft(){

  // asuro on the move
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(0,BASE);
  // right sensor sees black = line found
  asuro.readLinesensor(l_data);
  //if (l_data[1] < DARK){
  //  a_state = FOLLOW_LINE;
  //  asuro.setMotorSpeed(0,0);
  //  return;
//  }
      asuro.readOdometry(odo_data);
      findTick(1);

      if(ticks[1] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving

        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
        return;
      }


}
void turnRightBack(){
  asuro.readOdometry(old_odo_data);
  asuro.setMotorDirection(RWD,FWD);
  asuro.setMotorSpeed(BASE,0);
  asuro.readOdometry(odo_data);
      findTick(0);
      Serial.print(ticks[0]);
      if(ticks[0] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving
        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
          return;
      }
}
void turnLeftBack(){
  asuro.readOdometry(old_odo_data);
  asuro.setMotorDirection(FWD,RWD);
  asuro.setMotorSpeed(0,BASE);
  asuro.readOdometry(odo_data);
      findTick(1);
      Serial.print(ticks[1]);
      if(ticks[1] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving

        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
          return;;
      }



}
void searchLineRoutine(){
  // turn left
  turnLeft();
  if (a_state != SEARCH_LINE) return;
  turnLeftBack();
  if (a_state != SEARCH_LINE) return;
  turnRight();
  if (a_state != SEARCH_LINE) return;
  turnRightBack();
  // turn left back
  asuro.setMotorSpeed(0,0);
  a_state = SCAN_BARCODE;
  //
}

void setup(){
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
    asuro.setFrontLED(ON);
  delay(200);
  Serial.print("Hardware Ready");
  }

  void loop(){
    asuro.readOdometry(old_odo_data);
    ticks[0] = 0;
    ticks[1] = 0;

    speed[0] = BASE;
    speed[1] = BASE;
    b_state = BRIGHT;
    a_state = SEARCH_LINE;

    while(1){
      if (a_state = SEARCH_LINE){
        searchLineRoutine();
      }
    }
  }
