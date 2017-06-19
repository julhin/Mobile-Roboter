#include <Asuro.h>

#define MAX_SPEED 120
#define BREAK_DELTA 5
#define FORWARD_DELTA 20

  Asuro asuro = Asuro();
 
  
 

  /**
   * This function slows down a wheel (by using a break_delta vlaue) and
   * sets  the speed of the other wheel to max_speed
   * identifier = 0: left Wheel will be breaked
   * identifier = 1: right Wheel will be breaked
    */
 void breakWheel(unsigned int wheelSpeed[], unsigned int identifier){
    
    if (wheelSpeed[identifier] > BREAK_DELTA){
      wheelSpeed[identifier] -= BREAK_DELTA;  // decelerate Wheel
      
    } else {
     wheelSpeed[identifier] = 0;  // to prevent underflow
    }

    wheelSpeed[(identifier + 1)%2] = MAX_SPEED;  //change the speed of the other wheel to max speed
  }


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
      asuro.setBackLED(OFF,OFF);               //Breaklights
      
     }else if(lineData[0] > lineData[1]){      //right turn
       breakWheel(wheelSpeed,1 );              //break right wheel
       asuro.setBackLED(OFF,ON);               
       
     }else{                                    //Left turn
      breakWheel(wheelSpeed,0);
      asuro.setBackLED(ON,OFF);
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
