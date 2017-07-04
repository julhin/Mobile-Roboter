#include <Asuro.h>

/**
 * Those values need to be improved!
 */
#define MAX_SPEED 100
#define BREAK_DELTA 60
#define LIGHT_DELTA 110 //has to be improved empiricallly

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
  void lineFollower(unsigned int lineData[],unsigned int oldLineData[], unsigned int wheelSpeed[]){
    int diff = lineData[0] - lineData[1];
    diff = abs(diff);  //workaround!  No Calculation within abs() allowed
     /**
      * if delta of both sensor is big enough -> asuro detected the edge of the line
      */
     if(diff > LIGHT_DELTA){                 // straight forward
      wheelSpeed[0] = MAX_SPEED;
      wheelSpeed[1] = MAX_SPEED;
      asuro.setBackLED(OFF,OFF);               //Breaklights
      oldLineData[0] = lineData[0];            //copy array to update old values
      oldLineData[1] = lineData[1];

     /*
      * delta to small!   no line detected so old sensor values need to
      * be compared in order to do a left or a right turn!
      * Important: old values only get ovrewritten, if asuro is following the line!
      */
     }else{
        if(oldLineData[0] > oldLineData[1]){      // check previous calculation
          breakWheel(wheelSpeed,1 );              //right turn / break right wheel
          asuro.setBackLED(OFF,ON);
        }else{
           breakWheel(wheelSpeed,0);             //left turn / break left wheel
            asuro.setBackLED(ON,OFF);
        }


     }
  }


void setup() {

  asuro.Init();
  asuro.setFrontLED(ON);
  Serial.begin(2400);
  asuro.setMotorDirection(FWD,FWD);

}

void loop() {
  /**
   * INITIALIZATION BLOCK
   */
  unsigned int wheelSpeed[2]; //wheelSpeed[0] = left wheel, wheelSpeed[1] =  right wheel
  unsigned int lineData[2];
  unsigned int oldLineData[2];
  wheelSpeed[0] = MAX_SPEED;
  wheelSpeed[1] = MAX_SPEED;
  asuro.setMotorSpeed(wheelSpeed[0], wheelSpeed[1]);
  asuro.readLinesensor(lineData);
  asuro.readLinesensor(oldLineData);
  asuro.setBackLED(ON,ON);

  /**
   * actual program!
   */
  while(1){   // infinite loop

     asuro.readLinesensor(lineData);  //read sensor data
     lineFollower(lineData,oldLineData, wheelSpeed);  //calculate new motor speed
     asuro.setMotorSpeed(wheelSpeed[0], wheelSpeed[1]); //set new motor speed
     //read values here if necessary
  }

}
