#include <Asuro.h>

#define MAX_SPEED 80
#define PGAIN 1  //has to be improved empirically
#define FORWARD_DELTA 20

  Asuro asuro = Asuro();
 

  /**
   * this function is a three-step-controler
   *  - drive a left turn 
   *  - drive a right turn
   *  - drive straight forward
   *  
   */
  void lineFollower(unsigned int lineData[], unsigned int wheelSpeed[]){
    int diff = lineData[0] - lineData[1];
    diff = abs(diff);  //workaround!  No Calculation within abs() allowed
    
     if(diff < FORWARD_DELTA){                 // straight forward
      wheelSpeed[0] = MAX_SPEED;
      wheelSpeed[1] = MAX_SPEED;
      asuro.setBackLED(OFF,OFF);               
      
     }else if(lineData[0] > lineData[1]){      //break right/right turn
      
       wheelSpeed[0] = MAX_SPEED + PGAIN*diff;
       wheelSpeed[1] = MAX_SPEED - PGAIN*diff;
       asuro.setBackLED(OFF,ON);               //break lights
       
     }else{                                     //break left/left turn
     
       wheelSpeed[0] = MAX_SPEED - PGAIN*diff;
       wheelSpeed[1] = MAX_SPEED + PGAIN*diff;
      asuro.setBackLED(ON,OFF);                  //break lights       
     }
  }
  

void setup() {

  asuro.Init();
  asuro.setFrontLED(ON);
  Serial.begin(2400);
  asuro.setMotorDirection(FWD,FWD);

}

void loop() {
  unsigned int wheelSpeed[2]; //wheelSpeed[0] = left wheel, wheelSpeed[1] =  right wheel
  unsigned int lineData[2];
  wheelSpeed[0] = MAX_SPEED;  //initialize
  wheelSpeed[1] = MAX_SPEED; //initialize
  asuro.setMotorSpeed(wheelSpeed[0], wheelSpeed[1]); 
  
  while(1){   // infinite loop

     asuro.readLinesensor(lineData);  //read sensor data
     lineFollower(lineData, wheelSpeed);  //calculate new motor speed
     asuro.setMotorSpeed(wheelSpeed[0], wheelSpeed[1]); //set new motor speed
     //read values here if necessary
  }

}
