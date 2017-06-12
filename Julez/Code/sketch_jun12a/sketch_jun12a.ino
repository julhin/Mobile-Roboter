#include <Asuro.h>

unsigned int speed [2];
unsigned int line_sensors[2];
*int controlSpeed(){
readLinesensor(line_sensors);
/* if the sensors are more than 140 away  from each other, one sees black and the other sees black */
if (abs(line_sensors[0] - line_sensors[1]) < 100){
  if (line_sensors[0] < line_sensor[1]){
    /* left speed must be faster */
    speed[0] = 200;
    speed[1] = 100;
    
  }else if (line_sensors[0] > line_sensor[1]){
     speed[0] = 200;
    speed[1] = 100;
  }
}
  
}

void setup() {
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);

}

void loop() {
  // put your main code here, to run repeatedly:

}
