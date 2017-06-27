 
#include <Asuro.h>
#define HELL 0
#define DUNKEL 1
#define SEGMENTE 40
#define MAXSPEED 120
#define PGAIN 2
#define SCHWELLWERT 80


Asuro asuro = Asuro();

  struct umdrehung {    // Struct fuer die Berechnung der Umdrehung des linken und des rechten Rades
    unsigned int lastMax_val;      // Maxiler Hell ODER DUnkel Wert der letzten Messungen
    unsigned int farbeMittelwert; // Mittelwert, womit ueberprueft wird, ob die Scheibe gerade auf einem hellen oder dunklen Teil steht
    unsigned int encoder_ticks;   // zaehlt wieviele schwarz-weiß Uebergaenge die Scheibe bereits gemacht hat (60 = 1 Radumdrehung)
    unsigned int flag;        // Marker für Scheibenfarbe (HELL oder DUNKEL)
    unsigned int winkel; // augehend von 0 Grad
    };

  /*
   * Globale Variablen
   */
 
  unsigned int odo_data[2];     // Speicher für Odometriesensoren bereitstellen
  struct umdrehung links = {0,0, 0, DUNKEL, 0};
  struct umdrehung rechts = {0, 0, 0, DUNKEL, 0};
  
  

 /*
  * Methode zur Berechnung des aktuellen Winkels beziehungweie der ticks
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
    
      /*  Zurücksetzen wird nur bei Winkelmessung benutzt
      if (rad->encoder_ticks == SEGMENTE){
      rad->encoder_ticks = 0;
      rad->winkel = 0;
      } 
      rad->winkel +=9;
      */
      
     rad->encoder_ticks += 1;
    
     rad->flag = (rad->flag + 1)%2; //wird von 0 auf 1 oder von 1 auf 0 gesetzt
   }
   
   
   
 }

void setup() {
    asuro.Init();
    Serial.begin(2400);

   
    
    

}

void loop() {
      asuro.setMotorDirection(FWD,FWD);
      asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
      int diff;
      
    while(1){
     berechneWinkel(&links,0);
      berechneWinkel(&rechts,1);
      diff = links.encoder_ticks - rechts.encoder_ticks;

      /**
       * Hier ist der eigentliche P-Regler
       */
      if(diff < 0 ){
        asuro.setMotorSpeed(MAXSPEED + PGAIN*diff,MAXSPEED - PGAIN*diff);
      }else{
        asuro.setMotorSpeed(MAXSPEED - PGAIN*diff,MAXSPEED + PGAIN*diff);
      }

    }


}





