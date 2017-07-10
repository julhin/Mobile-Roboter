#include <Asuro.h>

Asuro asuro = Asuro();
#define KP 0.2
#define BASE 100

void p_regler(){
  asuro.readLinesensor(l_data);
  if (l_data[0] > WHITE && l_data[1] > WHITE){
    //  a_state = SEARCH_LINE;
      asuro.setMotorSpeed(0,0);
  } else {
  int diff = l_data[0] - l_data[1];
  speed[0] = BASE + diff * KP;
  speed[1] = BASE - diff * KP;
  }
  asuro.setMotorSpeed(speed[0], speed[1]);
  }

Asuro asuro = Asuro();
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

  }
