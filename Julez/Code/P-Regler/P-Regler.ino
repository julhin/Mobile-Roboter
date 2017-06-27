#include <Asuro.h>
/* Values need adjustment*/
#define BASE_SPEED 100
#define REASL_SPEED 1.0
#define FACTOR_SPEED 10
#define FACTOR_ODO 2
#define HELL 0
#define DUNKEL 1
#define HELL_DUNKEL_WECHSEL 80
#define PERIOD 100
#define UMFANG 12.5

Asuro asuro = Asuro();
/*global values */
unsigned int odo_data [2];
unsigned int old_odo_data [2]:

int current_state[2] {1, 1};
unsigned int ticks [2];
long timer;

unsigned int motor_speed [2];
/*
 * Motorgeschwindigkeitsberechnung
 * über den aktuell gedrehten winkel in einer Zeitspanne
 */
 void calcRealSpeed(float time_passed,int side){
 int angle = ticks[side] * 9;

 float length = (UMFANG/360) * angle;
 float speed = length / time_passed ;
  
 ticks[side] = 0;
 return speed * 36 ;

  
 }
/* 
 *  positionsRegler, mit eschwindigkeit als Eingabe(kaskadierend)
 */
 void p_regler(int speed){
   
    int odo_Diff = (ticks[0] - ticks[1]) * FACTOR_ODO;
    int speed_Diff = (REAL_SPEED - speed) * FACTOR_SPEED;

    /*P-Regler gleichung*/
    motor_speed[0] = BASE_SPEED + odo_Diff + speed_Diff;
    motor_speed[1] = BASE_SPEED - odo_Diff + speed_Diff;
 }
/*
   Tickszähler
*/
void findTicks(){
  if ( millis() - timer < 0) return;
    timer = millis();
    countTick(0);
    countTick(1);
  
}

void countTick(int side) {
  asuro.readOdometry(odo_data);
  int diff = abs ( old_odo_data[side] - odo_data[side]);

  switch (current_state[side]) {
    case HELL :
      // Still light, but a new value
      if (odo_data[side] < old_odo_data[side]) {
        old_odo_data[side] = odo_data[side];
        // change has happened,
      } else if (diff > HELL_DUNEKL_WECHSEL) {
        ticks[side] ++;
        current_state[side] = DUNKEL;
        old_odo_data[side] = odo_data[side];
      }
    case DUNKEL :
      if  (odo_data[side] > old_odo_data[side]) {
        old_odo_data[side] = odo_data[side];
      } else if (diff > HELL_DUNEKL_WECHSEL) {
        ticks[side] ++;
        current_state[side] = HELL;
        old_odo_data[side] = odo_data[side];
      }

  }
}

void setup() {
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
  asuro.setMotorDirection(FWD, FWD);
  asuro.setMotorSpeed(BASE, BASE);
  delay(2000);
  /*
   * set initial values
   */
   ticks = {0,0};
   motorspeed = {BASE_SPEED,BASE_SPEED}; 
}

void loop() {
  asuro.setMotorDirection(FWD, FWD);
  asuro.setMotorSpeed (BASE, BASE);

  long timer = millis();
    long time_diff = 0;
    float right_speed = 1.0;
    float left_speed = 1.0;
    float overall_speed = 1.0;
    
  while (1) {
    findTicks();
    time_diff = millis() - timer ;

    if (time_diff > 100){
      right_speed = calcRealSpeed(time_diff, 1);
      left_speed = calcRealSpeed(time_diff, 0);
      overall_speed (right_speed + left_speed) / 2.0;
    }
    
    p_regler(overall_speed); 
  }
}
