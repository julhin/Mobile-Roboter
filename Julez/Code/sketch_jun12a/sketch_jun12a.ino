#include <Asuro.h>

 Asuro asuro = Asuro();
unsigned int current_speed [2];
int line_sensors[2];

 void controlSpeed(){
asuro.readLinesensor(line_sensors);

/* if the sensors are more than 140 away  from each other, one sees black and the other sees black */
int temp = line_sensors[0] - line_sensors[1] ;
 temp = abs(temp);
if (temp < 20){
  if (line_sensors[0] < line_sensors[1]){
    /* left speed must be faster */
   current_speed[0] = 20;
   current_speed[1] = 10;
    
  }else if (line_sensors[0] > line_sensors[1]){
    /* right  has to be faster */
    current_speed[0] = 10;
    current_speed[1] = 20;
  }
}
}

void setup() {
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
 asuro.setMotorDirection(FWD,FWD);
 current_speed[1] = 20;
 current_speed[0] = 20;
 
}

void loop() {
  
  asuro.setMotorSpeed(20,20);
  while(1);
  controlSpeed();
  asuro.setMotorSpeed (current_speed [0], current_speed[1]); 
}
