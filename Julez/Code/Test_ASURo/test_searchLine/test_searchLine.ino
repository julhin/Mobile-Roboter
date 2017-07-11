#include <Asuro.h>

#define KP 0.2
#define BASE 100
#define TICKS_ANGLE 30
#define ODOTHRESHOLD 80
#define MAX_BARCODE_DISTANCE 10
#define DARK 100
#define WHITE 800
#define LINE_CONTRAST 80

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
enum TURN_STATE{
  LEFT_TURN,
  LEFT_TURN_BACK,
  RIGHT_TURN,
  RIGHT_TURN_BACK,
  STOP_TURNING_SCAN,
  STOP_TURNING_FOLLOW
};
TURN_STATE t_state;

bool is_on_line(){
    asuro.readLinesensor(l_data);
  //  Serial.print(l_data[0]);
  //  Serial.print(",");
  //  Serial.print(l_data[1]);
  //  Serial.print("\n");
    int diff = l_data[0] - l_data[1];
    diff = abs(diff);
    if(l_data[0] + l_data[1] < WHITE)
  return true;
  return false;
}

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
  asuro.readOdometry(odo_data);
  // asuro on the move
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(BASE,0);
  if (is_on_line()){
  //  a_state = FOLLOW_LINE;
    asuro.setMotorSpeed(0,0);
    a_state = FOLLOW_LINE;
    return;
  }
      asuro.readOdometry(odo_data);
      findTick(0);
      Serial.print(ticks[0]);
      Serial.print("\n");
      if(ticks[0] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving

        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
        t_state = RIGHT_TURN_BACK;
          return;
      }

}
void turnLeft(){

  // asuro on the move
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(0,BASE);
  // right sensor sees black = line found
  asuro.readLinesensor(l_data);
  if (l_data[1] + l_data[0] < WHITE){
    asuro.setStatusLED(RED);
    a_state = FOLLOW_LINE;
    t_state = STOP_TURNING_FOLLOW;
    asuro.setMotorSpeed(0,0);
    return;
  }
        asuro.readOdometry(odo_data);
        findTick(1);
        Serial.print(ticks[1]);
        Serial.print("\n");
      if(ticks[1] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving

        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
        t_state = LEFT_TURN_BACK;
        return;
      }


}
void turnRightBack(){

  asuro.setMotorDirection(RWD,FWD);
  asuro.setMotorSpeed(BASE,0);
  asuro.readOdometry(odo_data);
  findTick(0);
  Serial.print(ticks[1]);
  Serial.print("\n");
      if(ticks[0] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving
        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
        t_state = STOP_TURNING_SCAN;
        return;
      }
}
void turnLeftBack(){
  asuro.readOdometry(odo_data);
  asuro.setMotorDirection(FWD,RWD);
  asuro.setMotorSpeed(0,BASE);
  asuro.readOdometry(odo_data);
      findTick(1);
      Serial.print(ticks[1]);
      Serial.print("\n");
      if(ticks[1] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving
          asuro.setMotorSpeed(0,0);
          ticks[0] = 0;
          ticks[1] = 0;
          t_state = RIGHT_TURN;
          return;
      }
    }
void searchLineRoutine(){
  // turn left
  switch(t_state){
  case LEFT_TURN:
  turnLeft();
  break;
  case LEFT_TURN_BACK:
  turnLeftBack();
  break;
  case RIGHT_TURN:
  turnRight();
  break;
  case RIGHT_TURN_BACK:
  turnRightBack();
  break;
  case STOP_TURNING_SCAN:
    a_state = SCAN_BARCODE;
    asuro.setMotorSpeed(0,0);
  case STOP_TURNING_FOLLOW:
  a_state = FOLLOW_LINE;
  asuro.setMotorSpeed(0,0);
  default:
  asuro.setMotorSpeed(0,0);
  asuro.setBackLED(ON,ON);
  }
  //
}

void setup(){
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
  asuro.setFrontLED(ON);
  Serial.print("Hardware Ready");
    delay(200);
  }

  void loop(){
    asuro.readOdometry(old_odo_data);
    ticks[0] = 0;
    ticks[1] = 0;

    speed[0] = BASE;
    speed[1] = BASE;

    b_state = BRIGHT;
    a_state = SEARCH_LINE;
    t_state = LEFT;
    asuro.setMotorDirection(FWD,FWD);
    while(1){
      if (a_state = SEARCH_LINE){
        searchLineRoutine();
      } else {
        asuro.setStatusLED(YELLOW);
        asuro.setMotorSpeed(0,0);
        asuro.setBackLED(ON,ON);
      }
    }
  }
