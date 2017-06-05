 
#include <Asuro.h>
#define HELL 0
#define DUNKEL 1

Asuro asuro = Asuro();

  struct umdrehung {    // Struct fuer die Berechnung der Umdrehung des linken und des rechten Rades
    unsigned int farbeNeu;      // aktueller Wert der Odometriescheibe
    unsigned int farbeMittelwert; // Mittelwert, womit ueberprueft wird, ob die Scheibe gerade auf einem hellen oder dunklen Teil steht
    unsigned int uebergaenge;   // zaehlt wieviele schwarz-weiß Uebergaenge die Scheibe bereits gemacht hat (60 = 1 Radumdrehung)
    unsigned int flag;        // Marker für Scheibenfarbe (HELL oder DUNKEL)
    unsigned int winkel; // augehend von 0 Grad
    };

  /*
   * Globale Variablen
   */
 
  unsigned int odo_data[2];     // Speicher für Odometriesensoren bereitstellen
  struct umdrehung links = {0, 0, 0, DUNKEL, 0};
  struct umdrehung rechts = {0, 0, 0, DUNKEL, 0};

 /*
  * Methode zur Berechnung des aktuellen Winkels
  */
 void berechneWinkel(struct umdrehung *rad, int identifier){  // identifier 0 = linkes Rad, 1 = rechtes Rad
    asuro.readOdometry(odo_data);  // Odometrie auslesen fuer ein Rad
    rad->farbeNeu = odo_data[identifier]; //identifier für linken/rechten Sensor
    
   if((rad->farbeNeu > rad->farbeMittelwert) && (rad->flag == DUNKEL)){
     if (rad->uebergaenge == 40){
      rad->uebergaenge = 0;
      rad->winkel = 0;
     }
     rad->uebergaenge += 1;
     rad->winkel +=9;
     rad->flag = HELL;

   }
   else if((rad->farbeNeu < rad->farbeMittelwert) && (rad->flag == HELL)){
      if (rad->uebergaenge == 40){
      rad->uebergaenge = 0;
      rad->winkel = 0;
     }
      rad->uebergaenge += 1;
      rad->winkel +=9;
      rad->flag = DUNKEL;

   }
 }

void setup() {
    asuro.Init();
    Serial.begin(2400);

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
        counter++;

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
      asuro.setMotorSpeed(80,80);
      
    while(1){
      berechneWinkel(&links,0);
      berechneWinkel(&rechts,1);

    }


}





