#include <asuro.h>
#include <stdlib.h>

void p_regler(void){
  int diff = line_data[0] - line_data[1];
  motor_speed[0] = BASE + diff * GAIN;
  motor_speed[1] = BASE - diff *GAIN;
}

void follow(void){
  
}

int main(void){
  Init();

  while(1){

  }
}
