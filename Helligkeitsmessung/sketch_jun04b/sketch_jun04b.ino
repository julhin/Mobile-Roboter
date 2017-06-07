#include <Asuro.h>
Asuro asuro = Asuro();


void setup() {
  asuro.Init();
   Serial.begin(2400);

}

void loop() {
 unsigned int odoData[2];
 unsigned int leftOdoData[100];
 unsigned int rightOdoData[100];

 asuro.setMotorDirection(FWD,FWD);
 asuro.setMotorSpeed(70,70);

 for(int i=0; i<100;i++){
  asuro.readOdometry(odoData);
  leftOdoData[i] = odoData[0];
  rightOdoData[i] = odoData[1];
  delay(10);
 }

  asuro.setMotorSpeed(0,0);
 delay(4000);

 Serial.println("Messnummer,Linker Messwert,Rechter Messwert"); 
 for(int i=0; i<100;i++){

 
Serial.print(i);
Serial.print(",");
Serial.print(leftOdoData[i]);
Serial.print(",");
Serial.println(rightOdoData[i]);

 }

 
 
}
