#include <Asuro.h>

Asuro asuro = Asuro();
usigned long time;
unsigned int odoData[2];
enum Enc_state {black,white};
unsigned int encoderValues [2];
int odoThrehshold = 140;

int findTick(int side, int* odoData) {
  
  static int lastVal[2] {512, 512}; // darkest
 
  switch (Enc_state[side]) {
    case white:
      if (odoData[side] < lastVal[side]) {
        lastVal[side] = odoData[side]; // new lowest value
      }
      else if (odoData[side] > (lastVal[side] + odoThrehshold)) { // transition from bright to dark
        lastVal[side] = odoData[side];
        Enc_state[side] = black;
        // tick
        encoderValues[side] ++;
        return 1;
      }
      break;
    case black:
    if(odoData[side] > lasVal[side]){
      lastVal[side] = odoData[side];
      else if (odoData[side] < (lastVal[side] + 140)){
         lastVal[side] = odoData[side];
         Enc_state [side] =  white;
         encoderValues[side] ++;
         retuen 1;
      }
      break;
    }
    
  }
  return 0;
}

void calculateSpeed(int side){
unsigned long current_time = millis();
int speed ;
if ((current_time - time) > 100){
  // i suppose ticks * 0,3 are m/s
   speed = encoderValues[side] * 0.3
   time = current_time;
}
Serial.print (speed);
Serial.print('\n');
}

void setup() {
  /* for stability some delays */
 asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
  asuro.setMotorSpeed(100,100);
}

void loop() {

 while(1){
  readOdometry(odoData);
  Serial.print("Left");
  calculateSpeed(0);
  Serial.print("Right");
  calculateSpeed(1);
 
 }

}
