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
bool is_on_line(){
    asuro.readLinesensor(l_data);
    if(l_data[0] < DARK & l_data[1] < DARK)
  return true;
  return false;
}
void scanBarcode(){
  asuro.setMotorSpeed(BASE,BASE);
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

      switch(a_state){
        case BLINK:
        blink_N_Times();
        break;
        case SCAN_BARCODE:
        scanBarcode();
        break;
        default:
        asuro.setBackLED(ON,ON);
      }

    }
  }
