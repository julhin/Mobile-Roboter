#include <Asuro.h>

// type defs
enum ENC_STATE {black, white};

// globals
enum ENC_STATE Enc_state[2] {black, black};

unsigned long taskStatusLED_timeout;
unsigned long taskEncoder_timeout;

unsigned long encoderValues[2] {0, 0};

// params
unsigned long taskStatusLED_period = 100;
unsigned long taskEncoder_period = 5;
int odoThrehshold = 140;



Asuro asuro = Asuro();
int ticks [2];
unsigned long taskStatusLED_timeout;
unsigned long taskEncoder_timeout;
// params
unsigned long taskStatusLED_period = 100;
unsigned long taskEncoder_period = 5;
int odoThrehshold = 140;

// function for counting ticks

int findTick(int side, int* odoData) {
  // high raw values mean dark, low raw values mean bright
  static int lastVal[2] {512, 512}; // darkest
  // left
  switch (Enc_state[side]) {
    case white: // values are low
      if (odoData[side] < lastVal[side]) {
        lastVal[side] = odoData[side]; // new lowest value
      }
      else if (odoData[side] > (lastVal[side] + odoThrehshold)) { // transition from bright to dark
        lastVal[side] = odoData[side];
        Enc_state[side] = black;
        // tick
        encoderValues[side] ++;
        return 1;
      }
      break;
    case black:
      if (odoData[side] > lastVal[side]){
        lastVal[side] = odoData[side];
      } else if (odoData[side] < (lastVal[side] + odoThrehshold)){
        lastVal[side] = odoData[side];
        Enc_state[side] = white ;
        encoderValues[side]++;
      }
  }
  return 0;
}

void countingTicks(){

  if (taskEncoder_timeout > millis()) return;
  taskEncoder_timeout += taskEncoder_period;
  int odoData[2];
  asuro.readOdometry(odoData);

  int foundTick = 0;
  //sloppy workaround
  ticks[1] += findTick(1, odoData);
  ticks[0] += findTick(0, odoData);

}
// drive a rectangle
void drive_line_and_curve(){
  // count ticks
  bool temp_bool = true;
  while(1){
  countingTicks();
  current_ticks = max(ticks[0], ticks[1]);
  Serial.print(ticks[0]);
  Serial.print(",");
  Serial.print(ticks[1]);
  if (current_ticks < 40){
    // drive STRAIGHT
    asuro.setMotorDirection(FWD,FWD);
    asuro.setMotorSpeed(100,100);
  } else if (current_ticks > 39 && current_ticks < 67){
    // drive a 90° angle
    asuro.setMotorDirection(FWD,FWD);
    asuro.setMotorSpeed(0,100);
  } else {
    ticks[1] = 0;
    ticks[0] = 0;
    temp_bool = false;
  }
}
}


void setup(){
asuro.Init();
delay(200);
Serial.begin(2400);
delay(200);
asuro.setMotorDirection(FWD,FWD);
asuro.setMotorSpeed(100,100);
}

void loop(){

while (1) {
   drive_line_and_curve();
   drive_line_and_curve();
   drive_line_and_curve();
   drive_line_and_curve();
}
}
