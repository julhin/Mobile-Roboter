#include <Asuro.h>
Asuro asuro = Asuro();


void setup() {
  asuro.Init();
  Serial.begin(2400);

}

void loop() {
 unsigned int odoData[2];


 //asuro.setMotorDirection(FWD,FWD);
 //asuro.setMotorSpeed(70,70);

  int current = millis();
  int temp = millis();
  
  while(true){
    current = millis();
    if(current - temp > 5000){
      temp = millis();
      asuro.readOdometry(odoData);
      Serial.print("Links: ");
      Serial.println(odoData[0]);
      Serial.print("Rechts: ");
      Serial.println(odoData[1]);
  
    }
  }
  
  
 

 
 
}
