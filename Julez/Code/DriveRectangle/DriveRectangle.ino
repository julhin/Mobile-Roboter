#include <Asuro.h>


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
        Enc_state[side] = black ;
        encoderValues[]
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
  ticks[1] += findTick(RIGHT, odoData);
  ticks[0] +=  findTick(LEFT, odoData);

}
// drive a rectangle
void driveRectangle(){
  // count ticks
  countingTicks();
  current_ticks = max(ticks[0], ticks[1]);
  if (current_ticks < 40){
    // drive STRAIGHT
    asuro.setMotorDirection(FWD,FWD);
    asuro.setMotorSpeed(100,100);
  } else if (current_ticks > 39 && current_ticks < 67){
    // drive a 90° angle
    asuro.setMotorDirection(FWD,FWD);
    asuro.setMotorSpeed(0,100);

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
  for(int i = 0; i < 3 , i++){
    driveRectangle();
  }
}
}
