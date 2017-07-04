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

  while(1){
    asuro.readLinesensor(line_data);
    Serial.print(line_data[0]);
    Serial.print(",");
    Serial.print(line_data[1]);
    Serial.print("\n");
    delay(1000);
  }
}
