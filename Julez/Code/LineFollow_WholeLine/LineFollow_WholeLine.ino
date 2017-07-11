#include <Asuro.h>

#define KP 0.2
#define BASE 100
#define TICKS_ANGLE 40
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
  SEARCH_LINE_TRANS,
  FOLLOW_LINE,
  SCAN_BARCODE,
  SCAN_BARCODE_TRANS,
  BLINK,
  FIND_TIMBER,
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

bool first;
// TODO MAKE IT BETTER
void find_Timber(){
  asuro.setMotorSpeed(BASE-20,BASE-20);
  if (asuro.readSwitches() > 0){
    a_state = STOP_ASURO;
    asuro.setMotorSpeed(0,0);
  }
}
void stop_asuro(){
  asuro.setMotorSpeed(0,0);
  asuro.setStatusLED(GREEN);
}
void find_line(){

  asuro.setMotorSpeed(BASE,BASE);
  asuro.readLinesensor(l_data);
  asuro.readOdometry(odo_data);
  findTick(0);
  findTick(1);

  if (is_on_line()){
    a_state = FOLLOW_LINE;
  }
  if (ticks[0] + ticks[1] > 35){
    a_state = FIND_TIMBER;
  }
  }
void stop(){
  asuro.setMotorSpeed(0,0);
  asuro.setStatusLED(GREEN);
}
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
void scanBarcode(){
  asuro.setMotorSpeed(BASE-20,BASE-20);
  switch(b_state){
  case BRIGHT:
  //TODO read Switches to determine if the end is reached
  asuro.setStatusLED(YELLOW);
  asuro.readOdometry(odo_data);
  findTick(0);
  findTick(1);
  ticks_since_last_dark = ticks[0] + ticks[1];
  Serial.print(ticks_since_last_dark);
  if (ticks_since_last_dark > MAX_BARCODE_DISTANCE){
    //end reached
    b_state = STOP;
    break;
  }
  if(is_on_line()){
    b_state = BLACK;
    break;
  }
  break;

  case BLACK:
  if (!is_on_line()){
    asuro.setStatusLED(GREEN);
    barcode_count++;
    b_state = BRIGHT;
  }
  // reset Values
  ticks[0] = 0;
  ticks[1] = 0;
  break;

  case STOP:
  asuro.setMotorSpeed(0,0);
  a_state = BLINK;
  break;
  }
  }
void blink_N_Times(){
  asuro.setStatusLED(GREEN);
 //  Serial.print("Blinken");
 //  Serial.print("\n");
 Serial.print("COUNT: ");
  Serial.print(barcode_count);
 //  Serial.print("\n");

  delay(1000);
  for(int i = 0; i < barcode_count; i++){
    asuro.setBackLED(ON,ON);
    delay(500);
    asuro.setBackLED(OFF,OFF);
    delay(500);
  }
  a_state = FIND_LINE;
  delay(500);
 }
void turnRight(){
  asuro.readOdometry(odo_data);
  // asuro on the move
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(BASE,0);
  if (is_on_line()){
  //  a_state = FOLLOW_LINE;
    asuro.setMotorSpeed(0,0);
    t_state = STOP_TURNING_FOLLOW;
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
    a_state = SCAN_BARCODE_TRANS;
    asuro.setMotorSpeed(0,0);
  case STOP_TURNING_FOLLOW:
  a_state = FOLLOW_LINE;
  asuro.setMotorSpeed(0,0);
  default:
  asuro.setMotorSpeed(0,0);
  asuro.setBackLED(ON,ON);
  }
  //
  asuro.setStatusLED(YELLOW);
  delay (500);

}
void p_regler(){
  asuro.readLinesensor(l_data);
  if (l_data[0] > WHITE && l_data[1] > WHITE){
      a_state = SEARCH_LINE_TRANS;
      asuro.setMotorSpeed(0,0);
  } else {
  int diff = l_data[0] - l_data[1];
  speed[0] = BASE + diff * KP;
  speed[1] = BASE - diff * KP;
  }
  asuro.setMotorSpeed(speed[0], speed[1]);
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
  ticks[0] = 0;
  ticks[1] = 0;
  speed[0] = BASE;
  speed[1] = BASE;
  barcode_count = 0;
  b_state = BRIGHT;
  a_state = SEARCH_LINE;
  t_state = LEFT;

  asuro.readOdometry(old_odo_data);

  while(1){
  switch(a_state){
  case  FIND_LINE: find_line();
  break;
  case SEARCH_LINE_TRANS:
  asuro.readOdometry(old_odo_data);
  ticks[0] = 0;
  ticks[1] = 0;
  a_state = SEARCH_LINE;
  case SEARCH_LINE:
  searchLineRoutine();
  break;
  case  FOLLOW_LINE:
   p_regler();
  break;
  case  SCAN_BARCODE_TRANS:
  barcode_count = 0;
  ticks_since_last_dark = 0;
  b_state = BRIGHT;
  a_state = SCAN_BARCODE;
  case SCAN_BARCODE:
  scanBarcode();
  break;
  case  BLINK:

  blink_N_Times();
  break;
  case  STOP_ASURO: stop_asuro();
  break;

  }
  }
  }
