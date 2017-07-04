#include <asuro.h>
#include <stdlib.h>
//TODO alle funktionen kontinuierlich machen, Übergänge zwischen den Zuständen
// die globalen Werte zurücksetzen
// Konstanten
#define BASE_SPEED 120
#define KP 0.1
// Has to be improved
#define ODO_THRESHOLD 140
#define WHITE
#define BLACK
#define TICK_THRESHOLD
// global variables
unsigned int line_data [2];
unsigned int odo_data[2];

unsigned int odo_data_old[2];
unsigned int speed[2];

unsigned int ticks[2];
unsigned int ticks_backwards [2];
enum TURN_STATE {LEFT, LEFT_BACK, RIGHT , RIGHT_BACK};
TURN_STATE t_sate;

unsigned int encoder_ticks_angle;
enum BARCODE_STATE {BRIGHT,DARK,STOP};
BARCODE_STATE b_state;
unsigned int ticks_since_last_dark;
int blink_times;
// PI-Regler zum linienfolgen, I Teil fehlt noch
void followLine(){

LineData(line_data);
// Anpassungen da der eine Sensor blöd gelötet ist
//+++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++
// line lost
if (line_data[0] > WHITE && line_data[1] > WHITE){
  // switch to state searchline
  searchLine();
} else {
int diff = line_data[0] - line_data[1];
// 0 -> left wheel, 1 -> right wheel
speed[0] += diff * KP;
speed[1] -= diff * KP;
}
}
// 90 degree turn in both directions
void searchLine(){
//
OdometrieData(odo_data);
//TODO switch syntax
switch(t_sate)
case RIGHT:
int diff_odo_right = abs(odo_data[1] - odo_data_old[1]);
if (diff_odo_right > ODO_THRESHOLD)
// tick happened
ticks[1]++;
if (ticks[1] > encoder_ticks_angle){
  // turn completed
  t_state = RIGHT_BACK;
  //
  break;
}
setMotorDirection(FWD,FWD);
setMotorSpeed(0,120);
break;

case LEFT:
int diff_odo_left = abs(odo_data[0] - odo_data_old[0]);
if (diff_odo_left > ODO_THRESHOLD)
// tick happened
ticks[0]++;
if (ticks[0] > encoder_ticks_angle){
  // turn completed
  t_state = LEFT_BACK;
  break;
}
setMotorDirection(FWD,FWD);
setMotorSpeed(120,0);
break;

case LEFT_BACK:
int diff_odo_left = abs(odo_data[0] - odo_data_old[0]);
if (diff_odo_left > ODO_THRESHOLD)
// tick happened
ticks_backwards[0]++;
if (ticks_backwards[0] > encoder_ticks_angle){
  // turn completed
  t_state = LEFT_BACK;
  //The search is completed
  scanBarcode();
  break;
setMotorDirection(RWD,FWD);
setMotorSpeed(120,0);
break;

// rechts wurde gesucht, als nächstes wird links gesucht
case RIGHT_BACK:
int diff_odo_right = abs(odo_data[1] - odo_data_old[1]);
if (diff_odo_right > ODO_THRESHOLD)
// tick happened
ticks_backwards[1]++;
if (ticks_backwards[1] > encoder_ticks_angle){
  // turn completed
  t_state = LEFT;
  //
  break;
}
setMotorDirection(FWD,FWD);
setMotorSpeed(0,120);
break;

}
}
// not sure, if you are needed
void findline(){
}

void blinkNTimes(){
BackLED(OFF,OFF);
int i;
for (i = 0; i < blink_times; i++){
  BackLED(ON,ON);
  // slepp 100 ms
  BackLED (OFF,OFF);
  //sleep 100 ms
}
}
// sloppy solution, i could use traktorie um die fahrt ruhiger zu machen
void scanBarcode(){
LineData(line_data);
// speed the Asuro up, so he can read the BARCODE_STATE
MotorDir(FWD,FWD);
MotorSpeed(BASE_SPEED,BASE_SPEED);
switch (b_state) {
  case DARK:
  //+++++++++++++++++++++++++++++++++++
 // offset berechnung
  //+++++++++++++++++++++++++++++++++++
  // Drive until WHITE is seen

  if (line_data[0] > WHITE && line_data[1] > WHITE){
    // barcode ende ,
    blink_times++;
    b_state = BRIGHT;
  }
  break;

  case BRIGHT:
  // count ticks
  if (ticks_since_last_dark > TICK_THRESHOLD)
  b_state = STOP;
  //+++++++++++++++++++++++++++++++++++
 // offset berechnung
  //+++++++++++++++++++++++++++++++++++
  //
  if(line_data[0] < BLACK && line_data[1] < BLACK){
  //
  ticks_since_last_dark = min(arr_ticks[0], arr_ticks[1]);
  b_state = DARK;
  break;
 }
 arr_ticks[0] += find_tick(0);
 arr_ticks[1] += find_tick(1);
  break;

  case STOP:
  blinkNTimes();
  return;

  default: // error handling :)
}

}
int find_tick(){
  int diff = abs(line_data[0] - line_data[1]);

  if (diff > TICK_THRESHOLD)
  return 1;
  return 0;
}
// maybe transistions

void main(){
    Init();
    // TODO initialisiere ALLE VAriablen
}
