#include <Asuro.h>

Asuro asuro = Asuro();

unsigned int line_data[2];

void setup(){
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
    asuro.setFrontLED(ON);
  delay(200);
}

void loop(){
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(80,80);
  while(1){
    for(int i = 0; i < 100; i++){
    asuro.readOdometry(line_data);    
    Serial.print(line_data[0]);
    Serial.print(",");
    Serial.print(line_data[1]);
    Serial.print("\n");
    delay(1000);
  }
  break;
}
}
