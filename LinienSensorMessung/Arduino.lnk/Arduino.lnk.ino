#include <Asuro.h>

Asuro asuro = Asuro();


void setup() {

  
asuro.Init();
Serial.begin(2400);
asuro.setFrontLED(ON);
}

void loop() {
  unsigned int startTime = millis();
  unsigned int currentTime;
  unsigned int data[2];
 while(1){
  currentTime = millis();
  if(currentTime - startTime >1500){
    asuro.readLinesensor(data);
    Serial.print("Der Linke Sensor:  ");
    Serial.println(data[0]);

     Serial.print("Der Rechte Sensor:  ");
    Serial.println(data[1]);

    
  Serial.print("Zusammen: ");
  Serial.println(data[0] + data[1]);
    startTime = millis();
  }
  
 }

}
