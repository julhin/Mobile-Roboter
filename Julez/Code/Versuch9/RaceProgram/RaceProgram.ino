#include <Asuro.h>
#define THRESHOLD 80

Asuro asuro = Asuro();

enum STATE {scanBarcode, followLine, searchLine, blinkNTimes,findLine, finish}
STATE state;


// global variables 
usigne dint line_data[2];
unsigned int last_line_data[2];
unsigned int maxVal [2];


// scnaBarcode Variables
int barcodeCount_Soll;
int barcodeCount;
int ticksSinceLastDark;
enum BARCODESTATE {BRIGHT,DARK,END};
BARCODESTATE b_state;


void task_scanBarcode(){
  if(  state != scanBarcode) return;
  //i assume the asuro stands still
  asuro.readLinesensor(last_line_data); 
  b_state = BRIGHT;
  while(1){
    // vorwärts fahren, 100 ms 
    
    asuro.setMotorDirection(FWD,FWD);
    asuro.setMotorSpeed (120,120);
    // sensoren auslesen und  
    asuro.readLinesensor(line_data);
    if (counting_ticks){
      
    }
    // MAxValues updaten falls notwendig
    if (line_data[0] < maxVal[0]) maxVal[0] = line_data[0];
    if (line_data[1] < maxVal[1]) maxVal[1] = line_data[1];

    // Übergang berechnen 
    int temp = line_data[0] - line_data[1];
  int temp = abs(temp);
   if ( temp > THRESHOLD ){ // Switch happened
    if (b_state == DARK){ // war schwarz, wird wieder weiß , zähle Barcode Strich
    barcodeCount++;
    counting_ticks = true;
    b_state = BRIGHT;
    }
    else if (b_state == BRIGHT) { 
      if (ticksSinceLastDark > 25 ){
        b_state = END;
        break;
      } else {
        
      }
      
    }
  }
  }
  // blinkNTimes
  for (int i =0; i < barCodeCount; i++){
    asuro.setBackLED(ON,ON);
    delay(200);
    asuro.setBackLED(OFF,OFF);
  }
  task_searchLine();
}

void setup() {
   asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
 // initialisize global variables
 barcodeCount_Soll = 3;
}

void loop() {
  // put your main code here, to run repeatedly:

}
