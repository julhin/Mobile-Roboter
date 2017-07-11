#include <Asuro.h>
#define KP 0.1
#define KI 0.1
#define TA 0.1
#define BASE 60
Asuro asuro =  Asuro();

unsigned int line_data [2];
int integral_diff;
unsigned int current_speed[2];

void p_regler(void){
  asuro.readLinesensor(line_data);

}
void pi_regler(void){
  // read Sensors
asuro.readLinesensor(line_data);
  // p-teil, aktuelle Abweichung
int diff = line_data[0] - line_data[1];
// I- Teil
float integral = integral_diff + diff;
if (integral > 255) integral = 255;
integral_diff = integral;
// Abtastrate
//pi_regler
int p_links = current_speed[0] + diff * KP + KI * TA * integral ;
int p_rechts = current_speed[1] -  diff * KP + KI * TA * integral;
current_speed[0] = p_links;
current_speed[1] = p_rechts;
}

void setup() {
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
  


}

void loop() {
// make the Asuro Move

while(1){
  asuro.setMotorDirection(FWD,FWD);
  asuro.setMotorSpeed(200,200);
}

}
