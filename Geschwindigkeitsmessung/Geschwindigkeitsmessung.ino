#include <Asuro.h>
#define HELL 0
#define DUNKEL 1
#define SEGMENTE 40
#define RADUMFANG 12.5

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
  unsigned int schwellenwert = 80; //Sinnvoller Wert nach erhobenen Daten

/*
 * Methode zur Berechnung der aktuellen Geschwindigkeit
 * Parameter:
 *   - timeDiff: Zeitperiode der Messung
 */
float berechneGeschwindigkeit(float timeDiff, struct umdrehung *rad){
    float strecke = ((RADUMFANG/360) * (rad->winkel));
    float geschwindigkeit = strecke / timeDiff;
    rad -> winkel = 0; //Setze Winkel des Rades zurück
    return geschwindigkeit * 36.0; //   cm pro millisekunde umgerechnet in km/h 
  
}


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
    
   if (diff > schwellenwert){  //wenn ja, dann Übergang!
    
    
      
     rad->encoder_ticks += 1;
     rad->winkel +=9;  //pro Übergang kommen 9° hinzu
     rad->flag = (rad->flag + 1)%2; //wird von 0 auf 1 oder von 1 auf 0 gesetzt
   }
  
 }

void setup() {
    asuro.Init();
    Serial.begin(2400);

}

void loop() {
  
      asuro.setMotorDirection(FWD,FWD);
      asuro.setMotorSpeed(255,255);
      long timer = millis();
      long timeDiff = 0;
      
    while(1){
     /*
      * ########################################################################
      * ##############Odometrie auslesen und verarbeiten########################
      */
     berechneWinkel(&links,0);
     berechneWinkel(&rechts,1);

     /*
      * ########################################################################
      * ##############Geschwindigkeit regeln####################################
      */
     if(links.encoder_ticks < rechts.encoder_ticks){
        asuro.setMotorSpeed(255,200);
      }else{
        asuro.setMotorSpeed(200,255);
      }

       /*
      * ########################################################################
      * ##############Geschwindigkeit berechnen und ausgeben####################
      */
      timeDiff = millis() -  timer;
         
      if(timeDiff > 500){ // Alle 500ms Ausgabe der Geschwindigkeit
        float rechteRadgeschwindigkeit = berechneGeschwindigkeit(timeDiff, &links);
        float linkeRadgeschwindigkeit = berechneGeschwindigkeit(timeDiff, &rechts);
        float endGeschwindigkeit = (rechteRadgeschwindigkeit + linkeRadgeschwindigkeit) / 2.0;  //Berechne Mittelwert rechtes und linkes Rad
        Serial.println(endGeschwindigkeit);
        timer = millis();  //reset timer
      }

    }


}





