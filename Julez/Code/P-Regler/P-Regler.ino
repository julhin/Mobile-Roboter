#include <Asuro.h>
/* Values need adjustment*/
#define BASE 100
//#define FACTOR 3
//#define SWITCH_BLACK_WHITE 23
Asuro asuro = Asuro();
/*global values */
//unsigned int delta;
//unsigned int soll_Wert;
//unsigned int ist_Wert;

//unsigned int odo_data [2];
int line_data [2];
//unsigned int speed_data[2];

/*alle benötigten Informationen sind vorhanden */
void p_Regler(){
asuro.readLinesensor(line_data);

int diff = line_data[0] - line_data[1];
int leftSpeed = BASE + diff ;
int rightSpeed = BASE - diff;

asuro.setMotorSpeed(leftSpeed, rightSpeed);
}
/*
void measureDelta(){
  for (int i = 0; i < 500; i++){
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed (255,255);
  }
   asuro.readLinsensor(line_data);
   delta = abs(line_data[0] - line_data[0]);
    asuro.setMotorSpeed (0,0);
}
*/

void setup() {
asuro.Init();
 delay(200);
 Serial.begin(2400);
 delay(200);
 asuro.setMotorDirection(FWD,FWD);
 asuro.setMotorSpeed(BASE,BASE);
  delay(2000);
}

void loop() {
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed (BASE,BASE);
while(1){
  p_Regler();
}

}