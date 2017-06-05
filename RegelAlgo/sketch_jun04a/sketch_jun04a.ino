 
#include <Asuro.h>
#define HELL 0
#define DUNKEL 1

Asuro asuro = Asuro();

  struct umdrehung {    // Struct fuer die Berechnung der Umdrehung des linken und des rechten Rades
    unsigned int farbeNeu;      // aktueller Wert der Odometriescheibe
    unsigned int farbeMittelwert; // Mittelwert, womit ueberprueft wird, ob die Scheibe gerade auf einem hellen oder dunklen Teil steht
    unsigned int uebergaenge;   // zaehlt wieviele schwarz-weiß Uebergaenge die Scheibe bereits gemacht hat (60 = 1 Radumdrehung)
    unsigned int flag;        // Marker für Scheibenfarbe (HELL oder DUNKEL)
    };

  /*
   * Globale Variablen
   */
  
  unsigned int odo_data[2];     // Speicher für Odometriesensoren bereitstellen
  struct umdrehung links = {0, 0, 0, DUNKEL};
  struct umdrehung rechts = {0, 0, 0, DUNKEL};


 void berechneUmdrehung(struct umdrehung *rad, int identifier){  // identifier 0 = linkes Rad, 1 = rechtes Rad
    asuro.readOdometry(odo_data);  // Odometrie auslesen fuer ein Rad
    rad->farbeNeu = odo_data[identifier];

   if((rad->farbeNeu > rad->farbeMittelwert) && (rad->flag == DUNKEL)){
     rad->uebergaenge += 1;
      rad->flag = HELL;

   }
   else if((rad->farbeNeu < rad->farbeMittelwert) && (rad->flag == HELL)){

      rad->uebergaenge += 1;
     rad->flag = DUNKEL;

   }
 }



void setup() {
    asuro.Init();
    Serial.begin(2400);

    /**
     * Mittelwert bestimmen
     */
     unsigned long aktZeit = millis();
     unsigned long tmpStartzeit = millis();
     asuro.setMotorDirection(FWD,FWD);
     asuro.setMotorSpeed(150,150);
     unsigned long tempMittelwertLinks = 0;
     unsigned long counterLinks = 0;
     unsigned long tempMittelwertRechts = 0;
     unsigned long counterRechts = 0;

     
     while(aktZeit - tmpStartzeit < 8000 ){

        asuro.readOdometry(odo_data);
      
        tempMittelwertLinks += odo_data[0];
        counterLinks++;

        tempMittelwertRechts += odo_data[1];
        counterRechts++;
        aktZeit = millis();
    }

    links.farbeMittelwert = tempMittelwertLinks/counterLinks;

    rechts.farbeMittelwert = tempMittelwertRechts / counterRechts;
    
    Serial.print("Der linke Sensor:  ");
    Serial.println(links.farbeMittelwert);
    Serial.print("Der rechte Sensor:  ");
    Serial.println(rechts.farbeMittelwert);

    


    asuro.setMotorSpeed(0,0);
    delay(1000);

  
   

}

void loop() {
      asuro.setMotorSpeed(100,100);
    while(1){
      berechneUmdrehung(&links,0);
      berechneUmdrehung(&rechts,1);
   
      if(links.uebergaenge < rechts.uebergaenge){
        asuro.setMotorSpeed(100,80);
      }else{
        asuro.setMotorSpeed(80,100);
      }

      
    }


}





