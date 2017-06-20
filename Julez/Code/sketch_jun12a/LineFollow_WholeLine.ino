#include <Asuro.h>
/* Values need adjustment*/
#define STRAIGHT_DRIVING 20
#define SPEED 100
#define BLACK 800
#define WHITE 200
#define SPEED_ALTERING_VALUE 5

Asuro asuro = Asuro();
int line_sensors[2];
unsigned int current_speed [2];

 void drive(){
   int diff = line_sensors[0] - line_sensors[1];
   diff = abs(diff);

   if (diff < STRAIGHT_DRIVING  ){
     current_speed[0]= SPEED;
     current_speed[1]= SPEED;
   } else if (line_sensors[0] < line_sensors[1] && line_sensors[1] > BLACK){
     // the left sensor is on white, while right sensor is on black
     // right need to be braked
     current_speed[0]= SPEED + SPEED_ALTERING_VALUE;
     current_speed[1]= SPEED - SPEED_ALTERING_VALUE;
   } else if (line_sensors[0] > line_sensors[1] && line_sensors[0] > BLACK){
   current_speed[0]= SPEED - SPEED_ALTERING_VALUE;
   current_speed[1]= SPEED + SPEED_ALTERING_VALUE;
 } else if (line_sensors[0] < WHITE && line_sensors[1] < WHITE){
 // line lost
      findLineAgain();
 }
 }

void findLineAgain(){
  asuro.setMotorDirection(FWD,RWD);
  asuro.setMotorSpeed(SPEED,SPEED);
}

void setup() {
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
 asuro.setMotorDirection(FWD,FWD);
 current_speed[0] = SPEED;
 current_speed[1] = SPEED;

}

void loop() {
  asuro.setMotorSpeed(SPEED,SPEED);
  while(1){
  asuro.readLinesensor(line_sensors);
  drive();
  asuro.setMotorSpeed (current_speed[0], current_speed[1]);
  }
}
