#include <asuro.h>
#define SOLL_SPEED 100

Asuro asuro = Asuro();

void generator(){
  
}
void setup() {
 asuro.Init();
 delay (200);
 Serail.begin(2400);
 delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:

}
