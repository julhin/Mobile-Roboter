 
#include <Asuro.h>
#define HELL 0
#define DUNKEL 1

Asuro asuro = Asuro();

  struct umdrehung {    // Struct fuer die Berechnung der Umdrehung des linken und des rechten Rades
    unsigned int last_val;      // aktueller Wert der Odometriescheibe
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
  unsigned int schwellenwert = 30; //DAS MUSS NOCH FESTGELEGT WERDEN!

 /*
  * Methode zur Berechnung des aktuellen Winkels
  */
 void berechneWinkel(struct umdrehung *rad, int identifier){  // identifier 0 = linkes Rad, 1 = rechtes Rad
    asuro.readOdometry(odo_data);  // Odometrie auslesen fuer ein Rad

    unsigned int diff = abs(rad->last_val - odo_data[identifier]); // Differenz berechnen
    
    rad->last_val = odo_data[identifier]; // neuen val setzen
   

   if (diff > schwellenwert){  //wenn ja, dann Übergang!

      if (rad->encoder_ticks == 40){
      rad->encoder_ticks = 0;
      rad->winkel = 0;
      }
      
     rad->encoder_ticks += 1;
     rad->winkel +=9;
     rad->flag = (rad->flag + 1)%2; //wird von 0 auf 1 oder von 1 auf 0 gesetzt
   }
   
   
   
 }

void setup() {
    asuro.Init();
    Serial.begin(2400);

}

void loop() {
      asuro.setMotorDirection(FWD,FWD);
      asuro.setMotorSpeed(100,100);
      
    while(1){
     berechneWinkel(&links,0);     // Hier kann der Winkel ausgelesen werden via  links.winkel  bzw rechts.winkel
      berechneWinkel(&rechts,1);
   
      

    }


}





