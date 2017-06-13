 
#include <Asuro.h>
#define HELL 0
#define DUNKEL 1
#define MAXSPEED 100
#define PGAIN 1
#define SEGMENTE 40
#define RADUMFANG 12

Asuro asuro = Asuro();

  struct umdrehung {    // Struct fuer die Berechnung der Umdrehung des linken und des rechten Rades
    unsigned int farbeNeu;      // aktueller Wert der Odometriescheibe
    unsigned int farbeMittelwert; // Mittelwert, womit ueberprueft wird, ob die Scheibe gerade auf einem hellen oder dunklen Teil steht
    unsigned int encoder_ticks;   // zaehlt wieviele schwarz-weiß Uebergaenge die Scheibe bereits gemacht hat (60 = 1 Radumdrehung)
    unsigned int flag;        // Marker für Scheibenfarbe (HELL oder DUNKEL)
    unsigned int winkel; // augehend von 0 Grad
    };

  /*
   * Globale Variablen
   */
 
  unsigned int odo_data[2];     // Speicher für Odometriesensoren bereitstellen
  struct umdrehung links = {0, 0, 0, DUNKEL, 0};
  struct umdrehung rechts = {0, 0, 0, DUNKEL, 0};
  boolean on;
  

 /*
  * Methode zur Berechnung des aktuellen Winkels
  */
 void berechneWinkel(struct umdrehung *rad, int identifier){  // identifier 0 = linkes Rad, 1 = rechtes Rad
    asuro.readOdometry(odo_data);  // Odometrie auslesen fuer ein Rad
    rad->farbeNeu = odo_data[identifier]; //identifier für linken/rechten Sensor
    
    if((rad->farbeNeu > rad->farbeMittelwert) && (rad->flag == DUNKEL)){
     rad->encoder_ticks += 1;
     //rad->winkel +=9;
     rad->flag = HELL;
     }
     else if((rad->farbeNeu < rad->farbeMittelwert) && (rad->flag == HELL)){
     rad->encoder_ticks += 1;
     //rad->winkel +=9;
     rad->flag = DUNKEL;
     }
 }

 void fahreGeradeaus(unsigned int cm){
  links.encoder_ticks=0;  //reset
  rechts.encoder_ticks=0;
 asuro.setMotorDirection(FWD,FWD);
 asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
       
 unsigned int encoderTicksSoll = (SEGMENTE / RADUMFANG) * cm;
 unsigned int encoderTicksHaben = 0;
 int diff; 
     
 while(encoderTicksHaben < encoderTicksSoll){
  
 berechneWinkel(&links,0);
 berechneWinkel(&rechts,1);
 diff = links.encoder_ticks - rechts.encoder_ticks;

  //Speed regeln
  if(diff < 0 ){
  asuro.setMotorSpeed(MAXSPEED + PGAIN*diff,MAXSPEED - PGAIN*diff);
  }else{
   asuro.setMotorSpeed(MAXSPEED - PGAIN*diff,MAXSPEED + PGAIN*diff);
  }
      
  encoderTicksHaben = ((links.encoder_ticks) + (rechts.encoder_ticks)) / 2;

  }
     
 
  }


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

  
      
 }


void setup() {
    asuro.Init();
    Serial.begin(2400);
    on = true;

    /**
     * Mittelwert Daten erheben
     */
     unsigned long aktZeit = millis();
     unsigned long tmpStartzeit = millis();
     asuro.setMotorDirection(FWD,FWD);
     asuro.setMotorSpeed(150,150);
     unsigned long tempMittelwertLinks = 0;
     unsigned long counter = 0;
     unsigned long tempMittelwertRechts = 0;
     
     while(aktZeit - tmpStartzeit < 8000 ){
      asuro.readOdometry(odo_data);
      tempMittelwertLinks += odo_data[0];
      tempMittelwertRechts += odo_data[1];
      counter++;
      aktZeit = millis();
    }

    /*
     * Mittelwert berechnen
     */
    links.farbeMittelwert = tempMittelwertLinks/counter;
    rechts.farbeMittelwert = tempMittelwertRechts / counter;
    
   
    asuro.setMotorSpeed(0,0);
    delay(1000);
   }


void loop() {
      links.encoder_ticks=0;  //reset
      rechts.encoder_ticks=0;
      
      while(on){
      
      
      fahreGeradeaus(40);
      asuro.setMotorSpeed(0,0);
      delay(1000);
      
      fahre90GradLinks();
      asuro.setMotorSpeed(0,0);
      delay(500);
      
      fahreGeradeaus(80);
      asuro.setMotorSpeed(0,0);
       delay(1000);
       
      fahre90GradLinks();
      asuro.setMotorSpeed(0,0);
      delay(500);
    
      fahreGeradeaus(80);
      asuro.setMotorSpeed(0,0);
       delay(1000);
       
      fahre90GradLinks();
      asuro.setMotorSpeed(0,0);
      delay(500);
      
      fahreGeradeaus(80);
      asuro.setMotorSpeed(0,0);
      delay(1000);
       
      fahre90GradLinks();
      
      asuro.setMotorSpeed(0,0);
      on = false;
       }
    


}





