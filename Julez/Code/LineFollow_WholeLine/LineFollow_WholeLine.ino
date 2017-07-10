#include <Asuro.h>
/* Values need adjustment*/
// P regler funktioniert bei mir besser als der Bauern ansatz
//
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
void find_line(){
  // drive 4 cm forward, then search the line , an dso on
  asuro.setMotorSpeed(BASE,BASE);
}
void stop(){
  asuro.setMotorSpeed(0,0);
  asuro.setStatusLED(GREEN);
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
  a_state = SCAN_BARCODE;
  //
}
bool is_on_line(){
    asuro.readLinesensor(l_data);
    if(l_data[0] < DARK & l_data[1] < DARK)
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
  switch(b_state){
  case BRIGHT:
  //TODO read Switches to determine if the end is reached
  asuro.readOdometry(odo_data);
  findTick(0);
  findTick(1);
  ticks_since_last_dark = ticks[0] + ticks[1];
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
  Serial.print("Blinken");
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
  asuro.readOdometry(old_odo_data);
  // asuro on the move
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(BASE,0);
  if (l_data[0] < DARK){
    a_state = FOLLOW_LINE;
    asuro.setMotorSpeed(0,0);
    return;
  }
      asuro.readOdometry(odo_data);
      findTick(0);
      Serial.print(ticks[0]);
      if(ticks[0] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving
        Serial.print("Hello");
        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
          return;
      }

  asuro.setStatusLED(YELLOW);
  delay (500);
}
void turnLeft(){

  // asuro on the move
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(0,BASE);
  // right sensor sees black = line found
  asuro.readLinesensor(l_data);
  if (l_data[1] < DARK){
    a_state = FOLLOW_LINE;
    asuro.setMotorSpeed(0,0);
    return;
  }
      asuro.readOdometry(odo_data);
      findTick(1);
      Serial.print(ticks[1]);
      if(ticks[1] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving
        Serial.print("Hello");
        asuro.setStatusLED(RED);
        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
        return;
      }


}
void turnRightBack(){
  asuro.readOdometry(old_odo_data);
  // asuro on the move
  asuro.setMotorDirection(RWD,FWD);
  asuro.setMotorSpeed(BASE,0);
  while(1){
      asuro.readOdometry(odo_data);
      findTick(0);
      Serial.print(ticks[0]);
      if(ticks[0] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving
        Serial.print("Hello");
        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
          return;
      }
  }
  asuro.setStatusLED(YELLOW);
  delay (500);

}
void turnLeftBack(){
  asuro.readOdometry(old_odo_data);
  // asuro on the move
  asuro.setMotorDirection(FWD,RWD);
  asuro.setMotorSpeed(0,BASE);
  while(1){
      asuro.readOdometry(odo_data);
      findTick(1);
      Serial.print(ticks[1]);
      if(ticks[1] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving
        Serial.print("Hello");
        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
          return;;
      }
  }
  asuro.setStatusLED(YELLOW);
  delay (500);

}
void p_regler(){
  asuro.readLinesensor(l_data);
  if (l_data[0] > WHITE && l_data[1] > WHITE){
      a_state = SEARCH_LINE;
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
  b_state = BRIGHT;
  //TODO : test purposes
  a_state =SEARCH_LINE;
  barcode_count = 0;
  asuro.setMotorDirection(FWD,FWD);
  asuro.setStatusLED(GREEN);
  asuro.setMotorSpeed(BASE,BASE);
  asuro.readOdometry(old_odo_data);
  while(1){
  switch(a_state){
    //functio nmissing
  case  FIND_LINE: find_line();
  break;
  // TODO: test
  case SEARCH_LINE: searchLineRoutine();
  break;
  // TODO : test
  case  FOLLOW_LINE: p_regler();
  break;
  // TODO : test
  case  SCAN_BARCODE:scanBarcode();
  break;
  case  BLINK: blink_N_Times();
  break;
  case  STOP_ASURO: stop();
  break;
  }
  }
  }
