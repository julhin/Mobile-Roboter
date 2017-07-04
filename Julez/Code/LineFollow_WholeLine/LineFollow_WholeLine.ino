#include <Asuro.h>
/* Values need adjustment*/
// P regler funktioniert bei mir besser als der Bauern ansatz
//
#define KP 0.2
#define BASE 100
#define TICKS_ANGLE 30
#define ODOTHRESHOLD 80

Asuro asuro = Asuro();

unsigned int l_data[2];
unsigned int speed[2];

unsigned int odo_data[2];
unsigned int old_odo_data[2];
unsigned int ticks[2];

void findTick(int id){
  int diff = odo_data[id] - old_odo_data[id];
  diff = abs(diff);
  if (diff > ODOTHRESHOLD ){
    // tick found
    ticks[id]++;
  }
  old_odo_data[id] = odo_data[id];
}
void turnLeft(){
  asuro.readOdometry(old_odo_data);
  // asuro on the move
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(0,BASE);
  while(1){
      asuro.readOdometry(odo_data);
      findTick(1);
      Serial.print(ticks[1]);
      if(ticks[1] > TICKS_ANGLE){
        // enough ticks, reset ticks, stop the asuro moving
        Serial.print("Hello");
        asuro.setMotorSpeed(0,0);
        ticks[0] = 0;
        ticks[1] = 0;
        break;
      }
  }
  asuro.setStatusLED(YELLOW);
  delay (500);

}

void p_regler(){
  asuro.readLinesensor(l_data);
  int diff = l_data[0] - l_data[1];
  speed[0] = BASE + diff * KP;
  speed[1] = BASE - diff * KP;
}

void setup(){
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
    asuro.setFrontLED(ON);
  delay(200);
}

void loop(){
  ticks[0] = 0;
  ticks[1] = 0;

  speed[0] = BASE;
  speed[1] = BASE;

  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(BASE,BASE);
    turnLeft();
 while(1){
   asuro.setMotorDirection(RWD,FWD);
   asuro.setMotorSpeed(BASE,BASE);
  //  p_regler();
  //  asuro.setMotorSpeed(speed[0], speed[1]);
  }

}
