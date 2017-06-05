#include <Asuro.h>
Asuro asuro = Asuro();
void setup() {
  asuro.Init();
    Serial.begin(2400);

}

void loop() {
 unsigned int odoData[2];

 while(1){

  
 asuro.readOdometry(odoData);
 Serial.print("Der linke Sensor:  ");
 Serial.println(odoData[0]);
 Serial.print("Der rechte Sensor:  ");
 Serial.println(odoData[1]);

 delay(10000);
 }
}
