#include <Asuro.h>
#define WHITETHRESHOLD 800  //has to be improved
#define MINLINECONTRAST 80
#define BARCODECONTRAST 50
#define ODOTHRESHOLD 80
#define BRIGHT 0
#define DARK 1
#define SEGMENTS 40
#define WHEELPERIMETER 12
#define MAXSPEED 120
#define MAXSPEEDTURN 100
#define PGAINFORWARD 2

#define BREAKDELTA 60

Asuro asuro = Asuro();
enum STATES{followLine, searchLine,scanBarcode, blinkNTimes,stop, findLine, error};

struct wheel {    // Struct for saving the state of the wheels
  unsigned int lastMax_val;      // Maxiler Hell ODER DUnkel Wert der letzten Messungen
  unsigned int encoder_ticks;   // counter for brigth/dark transitions (60 = 1 Radumdrehung)
  unsigned int flag;        //  current color
};

/*
 * Global Variables
 */
int barCount;
STATES current_state;
unsigned int odo_data[2];    
unsigned int lineData[2];
unsigned int oldLineData[2];
struct wheel left = {0,0, DARK};
struct wheel right = {0, 0, DARK};


/*
  * Method to calculate ticks, this is needed to drive straight  forward
  */
 void calculateTicks(struct wheel *rad, int identifier){  // identifier 0 = left wheel, 1 = tight wheel
    asuro.readOdometry(odo_data);  // Odometrie auslesen fuer ein Rad
    switch(rad->flag) {
      case DARK:
      if(odo_data[identifier] > rad->lastMax_val)  rad->lastMax_val = odo_data[identifier]; // neuer Max wenn vorher Dunkel
      break;
      case BRIGHT:
      if(odo_data[identifier] < rad->lastMax_val)  rad-> lastMax_val = odo_data[identifier]; // neuer Max wenn vorher Hell
      break;
      default:  //Error Handling?
      break;
    }
   unsigned int diff = rad->lastMax_val - odo_data[identifier]; // calculate difference
   diff = abs(diff); 
   if (diff > ODOTHRESHOLD){  //if so -> plus one tick!
   rad->encoder_ticks += 1;
   rad->flag = (rad->flag + 1)%2; //wird von 0 auf 1 oder von 1 auf 0 gesetzt
   }
 }
void driveStraight(){
    calculateTicks(&left,0);
    calculateTicks(&right,1);
    int diff = left.encoder_ticks - right.encoder_ticks;
    //Speed regeln
    if(diff < 0 ){
    diff = abs(diff);
    asuro.setMotorSpeed(MAXSPEED + PGAINFORWARD*diff,MAXSPEED - PGAINFORWARD*diff);
    }else{
    diff = abs(diff);
    asuro.setMotorSpeed(MAXSPEED - PGAINFORWARD*diff,MAXSPEED + PGAINFORWARD*diff);
    }    
    
}
void driveStraightCentimeter(unsigned int cm){
   left.encoder_ticks=0;  //reset
   right.encoder_ticks=0;
   asuro.setMotorDirection(FWD,FWD);
   asuro.setMotorSpeed(MAXSPEED,MAXSPEED);

   unsigned int encoderTicksDesiredValue = (SEGMENTS / WHEELPERIMETER) * cm;
   unsigned int encoderTicksCurrentValue = 0;
  
   while(encoderTicksCurrentValue < encoderTicksDesiredValue){
    driveStraight();
    encoderTicksCurrentValue = ((left.encoder_ticks) + (right.encoder_ticks)) / 2;
   }
  
 }
 


/*
 * Turn left/right (90 degree) forwards/backwards and scan for Line
 */
boolean turnLeftAndScanForLine(int leftwheel,int rightwheel){
  left.encoder_ticks=0;  //reset
  right.encoder_ticks=0;
  asuro.setMotorDirection(leftwheel, rightwheel);
  asuro.setMotorSpeed(MAXSPEEDTURN,MAXSPEEDTURN);
  unsigned int encoderTicksDesiredValue = 25;  //has to be improved
  unsigned int encoderTicksCurrentValue = 0; 
  while(encoderTicksCurrentValue < encoderTicksDesiredValue){
   
   if(isOnLine()) {   
   
   asuro.setMotorDirection(BREAK,BREAK);
   return true;
   }
   calculateTicks(&right,1);
   encoderTicksCurrentValue = right.encoder_ticks;
  }
  return false;
}

boolean turnRightAndScanForLine(int leftwheel,int rightwheel){
  left.encoder_ticks=0;  //reset
  right.encoder_ticks=0;
  asuro.setMotorDirection(leftwheel, rightwheel);
  asuro.setMotorSpeed(MAXSPEEDTURN,MAXSPEEDTURN);
  unsigned int encoderTicksDesiredValue = 25;  //has to be improved
  unsigned int encoderTicksCurrentValue = 0; 
  while(encoderTicksCurrentValue < encoderTicksDesiredValue){
    
   if(isOnLine()) {   
     
   asuro.setMotorDirection(BREAK,BREAK);
   return true;
   }
   calculateTicks(&left,0);
   encoderTicksCurrentValue = left.encoder_ticks;
  }
  return false;
}





 

/*
 * Start state: Drive straight, search Line 
 * Repeat procedure till line is found
 */
void task_findLine(){
  current_state = stop;
}

void task_followLine(){
  asuro.readLinesensor(lineData);  //read sensor data
  asuro.setMotorDirection(FWD,FWD);
   asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
  int diff = lineData[0] - lineData[1];
  diff = abs(diff);  //workaround!  No Calculation within abs() allowed
   /**
   * if delta of both sensor is big enough -> asuro detected the edge of the line
   */
  if(isOnLine()){    
   // if(diff > MINLINECONTRAST){               
   oldLineData[0] = lineData[0];            //copy array to update old values
   oldLineData[1] = lineData[1];
   asuro.setMotorDirection(FWD,FWD);
   asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
   }else{
    if(oldLineData[0] > oldLineData[1]){      // check previous calculation  
      asuro.setMotorSpeed(MAXSPEED,MAXSPEED-BREAKDELTA);
      current_state = searchLine;
      }else{
      asuro.setMotorSpeed(MAXSPEED - BREAKDELTA,MAXSPEED);
      current_state = searchLine;
      }
   }
}

void task_searchLine(){
  if(isOnLine()){ //check on more time if still on the line
    current_state = followLine;
    return;
  }
  asuro.setMotorDirection(BREAK,BREAK);
 
  //check previous calculation -> decision if left or right turn
  if(oldLineData[0] < oldLineData[1]){ 
    if(turnLeftAndScanForLine(RWD,FWD) || turnLeftAndScanForLine(FWD,RWD) || turnRightAndScanForLine(FWD,RWD) || turnRightAndScanForLine(RWD,FWD) ){  //scan starting left
    asuro.setMotorDirection(FWD,FWD);
    asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
    current_state = followLine;
    return;
    }
   current_state = scanBarcode;
   return;
  }else{
    if(turnRightAndScanForLine(FWD,RWD) || turnRightAndScanForLine(RWD,FWD) || turnLeftAndScanForLine(RWD,FWD) || turnLeftAndScanForLine(FWD,RWD) ){  //scan starting right
    asuro.setMotorDirection(FWD,FWD);
    asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
    current_state = followLine;
    return;
    }
   current_state = scanBarcode;
   return;
  }
}

/*
 * Scans for a line. Return true if found (edge of the line)
 */
boolean isOnLine(){
   asuro.readLinesensor(lineData);
   int diff = lineData[0] - lineData[1];
   diff = abs(diff);
   if((lineData[0] + lineData[1]) < WHITETHRESHOLD && diff > MINLINECONTRAST) {   //TODO: Werte anpassen?!
   //if(diff > MINLINECONTRAST) {  
   return true;
   }
   return false;
}


boolean isOnBarcode(){
   asuro.readLinesensor(lineData);
   int diff = lineData[0] - lineData[1];
   diff = abs(diff);
   if((lineData[0] + lineData[1]) < WHITETHRESHOLD && diff < BARCODECONTRAST) {   //TODO: Werte anpassen
  // Serial.println("Ist auf Barcode");
  // delay(1000);
   return true;
   
   }
   //Serial.println("Barcode false");
   return false;
}

/*
 * Drive straight for x cm.
 * Returns true as soon as the line was found
 * Returns false if line was not found within x cm
 */
boolean driveStraightAndScanForBarcode(unsigned int cm){
   left.encoder_ticks=0;  //reset
   right.encoder_ticks=0;
   asuro.setMotorDirection(FWD,FWD);
   asuro.setMotorSpeed(MAXSPEED,MAXSPEED);

   unsigned int encoderTicksDesiredValue = (SEGMENTS / WHEELPERIMETER) * cm;
   unsigned int encoderTicksCurrentValue = 0;
  
   while(encoderTicksCurrentValue < encoderTicksDesiredValue){
    if(isOnBarcode()) { 
    //Serial.print("Innerhalb driveStraight: ");
    //Serial.println(isOnBarcode());
    return true;
    }
    driveStraight();
    encoderTicksCurrentValue = ((left.encoder_ticks) + (right.encoder_ticks)) / 2;
   }
   //Serial.println(false);
   return false;
 }



void crossBarcode(){
   left.encoder_ticks=0;  //reset
   right.encoder_ticks=0;
   driveStraightCentimeter(3);
   
 }

void task_scanBarcode(){
 
  enum BARSTATES{onbar, offbar, goToBar};
  BARSTATES barstate = goToBar; //init
  barCount = 0; //reset 
  while(true){
    switch(barstate){
     case goToBar:
       if(driveStraightAndScanForBarcode(9)){
       barstate = onbar;
       }else{
       current_state = error;
       return;
       }
      break;
     case onbar:
      // Serial.println("onbar");
       crossBarcode();
       barCount++;
       barstate = offbar;
      break;
     case offbar:
       // Serial.println("driveStraightAndScanForBarcode: ");
        //Serial.println(driveStraightAndScanForBarcode(5));
       if(driveStraightAndScanForBarcode(5)){  //scan for next barcode
       //Serial.println("offbar");
       barstate = onbar;
       }else{
       //Serial.println("blinkNTimes");
       current_state = blinkNTimes;
       return; 
       }
      break;
    }
   }
   
}

void task_blinkNTimes(){
  Serial.println(barCount);
  asuro.setMotorDirection(BREAK,BREAK);
  if(barCount == 1){
    asuro.setBackLED(ON,ON);
    delay(500);
    asuro.setBackLED(OFF,OFF);
    current_state = stop;
    return;
  }else if(barCount == 0){
    current_state = error;
    return;
  }else{
    for(int i=0; i<barCount; i++){
    asuro.setBackLED(ON,ON);
    delay(500);
    asuro.setBackLED(OFF,OFF);
    delay(500);
    }
  }
  current_state = findLine;
}

void task_stop(){
    asuro.setMotorDirection(BREAK,BREAK);
      asuro.setStatusLED(GREEN);
}
void task_error(){
  asuro.setMotorDirection(BREAK,BREAK);
      asuro.setStatusLED(RED);
}
void task_default(){
  asuro.setMotorDirection(BREAK,BREAK);
      asuro.setStatusLED(RED);
      asuro.setBackLED(ON,ON);
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
  
  unsigned int lineData[2];
  unsigned int oldLineData[2];
  asuro.setMotorSpeed(MAXSPEED, MAXSPEED); 
  asuro.readLinesensor(lineData);
  asuro.readLinesensor(oldLineData); //for initial search direction
  current_state = searchLine; //set startState
  
  while(true){

    switch(current_state){

      case findLine: task_findLine(); break;
      case followLine: task_followLine(); break;
      case searchLine: task_searchLine(); break;
      case scanBarcode: task_scanBarcode();  break;
      case blinkNTimes: task_blinkNTimes(); break;
      case stop: task_stop(); break;
      case error: task_error();break;
      default: task_default();break; //this should not happen
      
    }
  }

}
