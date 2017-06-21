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
     //
    }
    // MAxValues updaten falls notwendig
    if (line_data[0] < maxVal[0]) maxVal[0] = line_data[0];
    if (line_data[1] < maxVal[1]) maxVal[1] = line_data[1];

    // Übergang berechnen 
    int temp = line_data[0] - line_data[1];
  int temp = abs(temp);
   if ( temp > THRESHOLD ){ // Switch happened
    
    switch (b_state){ // war schwarz, wird wieder weiß , zähle Barcode Strich
    case DARK:
    barcodeCount++;
    counting_ticks = true;
    b_state = BRIGHT;
    break;
    
    case: BRIGHT // wechsel zu schwarz
      if (ticksSinceLastDark > 25 ){
        b_state = END;
        break;  
      } else {
        ticksSinceLastDark = 0;
        countingTicks = false;
        b_state = DARK;
        continue;
      }
      break;  
  }
  }
  // blinkNTimes
  // ASURO anhalten
  asuro.setMotorSpeed(0,0);
  for (int i =0; i < barCodeCount; i++){
    asuro.setBackLED(ON,ON);
    delay(200);
    asuro.setBackLED(OFF,OFF);
  }
  task_searchLine();
}

void task_searchLine(){
  //sensoren überprüfen ob di eline verloren wurde 

  // linie rechts suchen 
  
  // falls gefunden => taks_followLine

  // falls nicht => Zurückfahren
  
  // linie links suchen 
  
  // falls gefunden => taks_followLine

  // falls nicht => Zurückfahren
  
  // task_scanBarcode() aufrufen

  
}
// TODO: make this method work
void fahre90GradLinks(){

    links.encoder_ticks=0;  //reset
     rechts.encoder_ticks=0;     
        asuro.setMotorDirection(FWD,FWD);
        asuro.setMotorSpeed(0,MAXSPEED);
     unsigned int encoderTicksSoll = 40;
     unsigned int encoderTicksHaben = 0; 
     while(encoderTicksHaben < encoderTicksSoll){
     berechneWinkel(&rechts,1); 
      encoderTicksHaben = rechts.encoder_ticks;
     
    }
// TODO: make thismethod work
void fahre90GradRechts(){

    links.encoder_ticks=0;  //reset
     rechts.encoder_ticks=0;   
        asuro.setMotorDirection(FWD,FWD);
        asuro.setMotorSpeed(MAXSPEED,0);
     unsigned int encoderTicksSoll = 40;
     unsigned int encoderTicksHaben = 0;   
     while(encoderTicksHaben < encoderTicksSoll){
     berechneWinkel(&rechts,1);
      encoderTicksHaben = rechts.encoder_ticks;
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
