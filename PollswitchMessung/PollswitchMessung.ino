#include <Asuro.h>
Asuro asuro = Asuro();


void setup() {
  asuro.Init();
   Serial.begin(2400);

}

void loop() {
 
  while(true){
   delay(500);
  int t1 = asuro.readSwitches();


  Serial.print("Switch: ");
  Serial.println(t1);
  
  
  
 }

  asuro.setMotorSpeed(0,0);
 delay(4000);




 
 
}
