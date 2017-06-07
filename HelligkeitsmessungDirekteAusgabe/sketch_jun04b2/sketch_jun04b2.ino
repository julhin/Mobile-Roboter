#include <Asuro.h>
Asuro asuro = Asuro();


void setup() {
  asuro.Init();
   Serial.begin(2400);

}

void loop() {
 unsigned int odoData[2];


 asuro.setMotorDirection(FWD,FWD);
 asuro.setMotorSpeed(70,70);
 Serial.println("Messnummer,Linker Messwert,Rechter Messwert"); 
 for(int i=0; i<1000;i++){

  
  asuro.readOdometry(odoData);
  
  Serial.print(i);
  Serial.print(",");
  Serial.print(odoData[0]);
  Serial.print(",");
  Serial.println(odoData[1]);
  
 }

  asuro.setMotorSpeed(0,0);
 delay(4000);




 
 
}
