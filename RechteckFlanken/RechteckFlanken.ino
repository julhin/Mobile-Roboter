 
#include <Asuro.h>
#define HELL 0
#define DUNKEL 1
#define SEGMENTE 40
#define RADUMFANG 12
#define MAXSPEED 120
#define PGAIN 2
#define SCHWELLWERT 80

Asuro asuro = Asuro();

  struct umdrehung {    // Struct fuer die Berechnung der Umdrehung des linken und des rechten Rades
    unsigned int lastMax_val;      // Maxiler Hell ODER DUnkel Wert der letzten Messungen
    unsigned int encoder_ticks;   // zaehlt wieviele schwarz-weiß Uebergaenge die Scheibe bereits gemacht hat (60 = 1 Radumdrehung)
    unsigned int flag;        // Marker für Scheibenfarbe (HELL oder DUNKEL)
    unsigned int winkel; // augehend von 0 Grad
    };

  /*
   * Globale Variablen
   */
 
  unsigned int odo_data[2];     // Speicher für Odometriesensoren bereitstellen
  struct umdrehung links = {0, 0, DUNKEL, 0};
  struct umdrehung rechts = {0,  0, DUNKEL, 0};
  boolean on;

 /*
  * Methode zur Berechnung des aktuellen Winkels
  */
 void berechneWinkel(struct umdrehung *rad, int identifier){  // identifier 0 = linkes Rad, 1 = rechtes Rad
    asuro.readOdometry(odo_data);  // Odometrie auslesen fuer ein Rad

    
    switch(rad->flag) {

      case DUNKEL:
       if(odo_data[identifier] > rad->lastMax_val)  rad->lastMax_val = odo_data[identifier]; // neuer Max wenn vorher Dunkel
       break;

       case HELL:
       if(odo_data[identifier] < rad->lastMax_val)  rad-> lastMax_val = odo_data[identifier]; // neuer Max wenn vorher Hell
       break;

       default:  //Error Handling?
       break;
    }
    
     unsigned int diff = rad->lastMax_val - odo_data[identifier]; // Differenz berechnen
     diff = abs(diff);
    
      if (diff > SCHWELLWERT){  //wenn ja, dann Übergang!
     
        // rad->winkel +=9; wird hier nicht benötigt!
        rad->encoder_ticks += 1;
        rad->flag = (rad->flag + 1)%2; //wird von 0 auf 1 oder von 1 auf 0 gesetzt
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
      diff = abs(diff);
      asuro.setMotorSpeed(MAXSPEED + PGAIN*diff,MAXSPEED - PGAIN*diff);
      }else{
         diff = abs(diff);
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
       

     unsigned int encoderTicksSoll = 35;
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

}

void loop() {
        
       
      while(on){
      
      
      fahreGeradeaus(40);
      asuro.setMotorSpeed(0,0);
      asuro.setMotorDirection(BREAK,BREAK);
      delay(1000);
      
      fahre90GradLinks();
      asuro.setMotorSpeed(0,0);
      asuro.setMotorDirection(BREAK,BREAK);
      delay(500);
      
      fahreGeradeaus(40);
      asuro.setMotorSpeed(0,0);
       asuro.setMotorDirection(BREAK,BREAK);
       delay(1000);
       
      fahre90GradLinks();
      asuro.setMotorSpeed(0,0);
       asuro.setMotorDirection(BREAK,BREAK);
      delay(500);
    
      fahreGeradeaus(40);
      asuro.setMotorSpeed(0,0);
       asuro.setMotorDirection(BREAK,BREAK);
       delay(1000);
       
      fahre90GradLinks();
      asuro.setMotorSpeed(0,0);
       asuro.setMotorDirection(BREAK,BREAK);
      delay(500);
      
      fahreGeradeaus(40);
      asuro.setMotorSpeed(0,0);
       asuro.setMotorDirection(BREAK,BREAK);
      delay(1000);
       
      fahre90GradLinks();
      
      asuro.setMotorSpeed(0,0);
       asuro.setMotorDirection(BREAK,BREAK);
      on = false;
       }
       
      
      
      
      
    


}





