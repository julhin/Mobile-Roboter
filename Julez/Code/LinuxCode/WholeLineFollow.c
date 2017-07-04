#include <asuro.h>
#include <stdlib.h>
/* Values need adjustment*/
#define STRAIGHT_DRIVING 20
#define SPEED 100
#define SPEED_ALTERING_VALUE 5

unsigned int line_sensors[2];
unsigned int current_speed [2];

 void drive(void){
   LineData(line_sensors);
   int diff = line_sensors[0] - line_sensors[1];
   diff = abs(diff);

   if (diff < STRAIGHT_DRIVING  ){
     current_speed[0]= SPEED;
     current_speed[1]= SPEED;
     // rechts sieht Weiß => linkes rad langsamer
   } else if (line_sensors[0] < line_sensors[1] ){
     if(current_speed[0] < SPEED_ALTERING_VALUE)
     current_speed[0] = SPEED_ALTERING_VALUE;
     current_speed[0]= current_speed[0] - SPEED_ALTERING_VALUE;
    current_speed[1]= SPEED;
    // links sieht weiß => rechts rad langsamer
   } else if (line_sensors[0] > line_sensors[1] ){
   current_speed[1]= current_speed[1] - SPEED_ALTERING_VALUE;
  current_speed[0]= SPEED;

// } else if (line_sensors[0] < WHITE && line_sensors[1] < WHITE){
//  findLineAgain();
 }
 }

void findLineAgain(){
//  asuro.setMotorDirection(FWD,RWD);
//  asuro.setMotorSpeed(SPEED,SPEED);
}

int main(void) {
// SETUP
  Init();
 MotorDir(FWD,FWD);
 current_speed[0] = SPEED;
 current_speed[1] = SPEED;

 MotorSpeed(SPEED,SPEED);
  while(1){
  LineData(line_sensors);
  drive();
  MotorSpeed(current_speed[0], current_speed[1]);
  }
}
