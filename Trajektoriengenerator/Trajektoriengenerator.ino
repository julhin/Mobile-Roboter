 
#include <Asuro.h>
#define HELL 0
#define DUNKEL 1
#define SEGMENTE 40
#define MAXSPEED 120
#define KMH 1.0
#define PGAINODO 2
#define PGAINKMH 10
#define SCHWELLWERT 80
#define RADUMFANG 12.5


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
  struct umdrehung links = {0,0, DUNKEL, 0};
  struct umdrehung rechts = {0, 0, DUNKEL, 0};
  
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
     rad->encoder_ticks += 1;
     rad->winkel +=9;
     rad->flag = (rad->flag + 1)%2; //wird von 0 auf 1 oder von 1 auf 0 gesetzt
    } 
 }


/*
 * Methode zur Anpassunjg der Position. Bekommt aktuelle Geschwindigkeit des ASUROs 
 * und passt dementsprechend die Geschwindigeit an
 */
void positionsRegler(int geschwindigkeit) {
   berechneWinkel(&links,0);
   berechneWinkel(&rechts,1);
   int odoDiff = (links.encoder_ticks - rechts.encoder_ticks)*PGAINODO;
   int speedDiff = (KMH - geschwindigkeit)*PGAINKMH;
    // Hier ist der eigentliche P-Regler der Odometrie
    if(odoDiff < 0 ){
    asuro.setMotorSpeed(MAXSPEED + odoDiff + speedDiff,MAXSPEED - odoDiff + speedDiff);
    }else{
    asuro.setMotorSpeed(MAXSPEED - odoDiff + speedDiff,MAXSPEED + odoDiff + speedDiff);
    }
    //Serial.println(MAXSPEED + odoDiff + speedDiff);

}

/*
 * Methode zur Berechnung der aktuellen Geschwindigkeit
 * Dies berechnet den ersten Regelparameter
 * Parameter:
 *   - timeDiff: Zeitperiode der Messung
 */
float berechneGeschwindigkeit(float timeDiff, struct umdrehung *rad){
    float strecke = ((RADUMFANG/360) * (rad->winkel));
    float geschwindigkeit = strecke / timeDiff;
    rad -> winkel = 0; //Setze Winkel des Rades zurück
    rad -> encoder_ticks = 0;
    return geschwindigkeit * 36.0; //   cm pro millisekunde umgerechnet in km/h 
}

void setup() {
    asuro.Init();
    Serial.begin(2400);
    asuro.setMotorDirection(FWD,FWD);
 
}


void loop() {
   asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
   /**
    * Timer + Geschwindigkeitsvariablen
    */
   long timer = millis();
   long timeDiff = 0;
   float rechteRadgeschwindigkeit =  1.0; //Initialisierung
   float linkeRadgeschwindigkeit = 1.0;
   float endGeschwindigkeit = 1.0;
  
   while(1){
   timeDiff = millis() -  timer; 
     if(timeDiff > 200){ // Alle 200ms Geschwindigkeit checken;
     rechteRadgeschwindigkeit = berechneGeschwindigkeit(timeDiff, &links);  
     linkeRadgeschwindigkeit = berechneGeschwindigkeit(timeDiff, &rechts);
     endGeschwindigkeit = (rechteRadgeschwindigkeit + linkeRadgeschwindigkeit) / 2.0;  //Regelwert 1
     timer = millis();  //reset timer
     }
     //Positionsregler bekommt aktuelle geschwindigkeit als Eingabe -> kaskadierter Regler
    positionsRegler(endGeschwindigkeit);
     
    }


}





