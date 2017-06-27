 
#include <Asuro.h>
#define HELL 0
#define DUNKEL 1
#define MAXSPEED 120
#define PGAIN 1

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
    /*Serial.print("farbeNeu ");
    Serial.println(rad->farbeNeu);
      Serial.print("farbeMittelwert ");
    Serial.println(rad->farbeMittelwert);
      Serial.print("Flag: ");
    Serial.println(rad->flag);
    Serial.println();*/
   if((rad->farbeNeu > rad->farbeMittelwert) && (rad->flag == DUNKEL)){
     
     rad->uebergaenge += 1;
     //rad->winkel +=9;
     rad->flag = HELL;

   }
   else if((rad->farbeNeu < rad->farbeMittelwert) && (rad->flag == HELL)){
       
      rad->uebergaenge += 1;
      //rad->winkel +=9;
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
     asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
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
    /*Serial.print("rechter Mittelwert ");
    Serial.println(rechts.farbeMittelwert);

     Serial.print("linker Mittelwert ");
    Serial.println(links.farbeMittelwert);*/

    
   
    asuro.setMotorSpeed(0,0);
    delay(1000);

  
   

}

void loop() {
      asuro.setMotorSpeed(MAXSPEED,MAXSPEED);
      int diff;
      
    while(1){
      berechneWinkel(&links,0);
      berechneWinkel(&rechts,1);
    
      diff = links.uebergaenge - rechts.uebergaenge;
      
      if(diff < 0 ){
        asuro.setMotorSpeed(MAXSPEED + PGAIN*diff,MAXSPEED - PGAIN*diff);
      }else{
        asuro.setMotorSpeed(MAXSPEED - PGAIN*diff,MAXSPEED + PGAIN*diff);
      }

    }


}





