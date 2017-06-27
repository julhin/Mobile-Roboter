#include <Asuro.h>

// Instantiate Asuro
Asuro asuro = Asuro();

// type defs
enum LED_STATE {eOn, eOff};
enum ENC_STATE {black, white};

// globals
enum LED_STATE taskStatusLED_state;
enum ENC_STATE Enc_state[2] {black, black};

unsigned long taskStatusLED_timeout;
unsigned long taskEncoder_timeout;

unsigned long encoderValues[2] {0, 0};

// params
unsigned long taskStatusLED_period = 100;
unsigned long taskEncoder_period = 5;
int odoThrehshold = 140;

// tasks
/************************************************/
void taskStatusLED() {
  // blink status led green @1Hz
  if (taskStatusLED_timeout > millis()) return;

  taskStatusLED_timeout += taskStatusLED_period;

  switch (taskStatusLED_state) {
    case eOn:
      asuro.setStatusLED(GREEN);
      taskStatusLED_state = eOff;
      break;

    case eOff:
      asuro.setStatusLED(OFF);
      taskStatusLED_state = eOn;
      break;
  }
}
/************************************************/
int findTick(int side, int* odoData) {
  // high raw values mean dark, low raw values mean bright
  static int lastVal[2] {512, 512}; // darkest
  // left
  switch (Enc_state[side]) {
    case white: // values are low
      if (odoData[side] < lastVal[side]) {
        lastVal[side] = odoData[side]; // new lowest value
      }
      else if (odoData[side] > (lastVal[side] + odoThrehshold)) { // transition from bright to dark
        lastVal[side] = odoData[side];
        Enc_state[side] = black;
        // tick
        encoderValues[side] ++;
        return 1;
      }
      break;
    case black: // values are high
      ........
  }
  return 0;
}

void taskEncoder() {
  // high raw values mean dark, low raw values mean bright
  static int highest[2] {512, 512}; // darkest
  static int lowest[2] {512, 512}; // brightest
  if (taskEncoder_timeout > millis()) return;
  taskEncoder_timeout += taskEncoder_period;
  int odoData[2];
  asuro.readOdometry(odoData);

  int foundTick = 0;
  foundTick += findTick(RIGHT, odoData);
  foundTick +=  findTick(LEFT, odoData);

  if (foundTick != 0) {
    Serial.print(encoderValues[LEFT]);
    Serial.print('\t');
    Serial.println(encoderValues[RIGHT]);
  }
}

// Init
/************************************************/
void hardwareInit() {
  // delays for robustness
  // using delays *before* the main loop is OK
  asuro.Init();
  delay(200);
  Serial.begin(2400);
  delay(200);
  Serial.println("Hardware ready");
  delay(200);
}
/************************************************/

// main
void setup() {
  hardwareInit();
  taskStatusLED_timeout = millis();
  taskEncoder_timeout = millis();
}

void loop() {
  taskStatusLED();
  taskEncoder();
}
