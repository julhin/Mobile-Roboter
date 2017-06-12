#include <Asuro.h>
/* Values need adjustment*/
#define FAST_SPEED 20
#define SLOW_SPEED 10

Asuro asuro = Asuro();
unsigned int current_speed [2];
int line_sensors[2];

 void controlSpeed(){
/* if the sensors are more than 20 away  from each other, one sees black and the other sees black */
int temp = line_sensors[0] - line_sensors[1] ;
 temp = abs(temp);
if (temp > 20){
  if (line_sensors[0] < line_sensors[1]){
    /* left speed must be faster */
   current_speed[0] = FAST_SPEED;
   current_speed[1] = SLOW_SPEED;
    
  }else if (line_sensors[0] > line_sensors[1]){
    /* right  has to be faster */
    current_speed[0] = SLOW_SPEED;
    current_speed[1] = FAST_SPEED;
  }
} else {
   current_speed[0] = FAST_SPEED; 
   current_speed[1] = FAST_SPEED;
}
}
void setup() {
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
 asuro.setMotorDirection(FWD,FWD);
 current_speed[1] = FAST_SPEED;
 current_speed[0] = FAST_SPEED;
 
}

void loop() {
  asuro.readLinesensor(line_sensors);
  asuro.setMotorSpeed(FAST_SPEED,FAST_SPEED);
  while(1);
  controlSpeed();
  asuro.setMotorSpeed (current_speed [0], current_speed[1]); 
}
