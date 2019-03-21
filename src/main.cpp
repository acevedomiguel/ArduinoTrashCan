#include <Arduino.h>
#include <Servo.h> 

const int SERVO_PIN = 9;
const int INPUT_PIN = 6;
const int S_DELAY = 500;
const int STALL_DELAY = 2000;
const int BUTTON_DELAY = 1000;
const int L_DELAY = 2000;

const int STATE_IDLE = 0;
const int STATE_OPEN = 1;
const int STATE_CLOSE = 2;
const int STATE_WAITING = 3;
const int STATE_STALL = 4;

const int OPEN_ANGLE = 0;
const int CLOSE_ANGLE = 45;

int state = STATE_IDLE;
int button_pressed = false;

Servo servo;

void m(int angle) {
  pinMode(SERVO_PIN, OUTPUT);
  servo.write(angle);
  delay(S_DELAY);
  pinMode(SERVO_PIN, INPUT);
}

void close() {
  m(CLOSE_ANGLE);
  state = STATE_IDLE;
}

void open() {
  state = STATE_OPEN;
  m(OPEN_ANGLE);

  state = STATE_WAITING;
  delay(BUTTON_DELAY);
  bool keep_open = (digitalRead(INPUT_PIN)==HIGH);

  if (keep_open) {
    Serial.println("Stalled");
  }

  delay(L_DELAY);

  if (keep_open) {
    delay(STALL_DELAY);
    state = STATE_STALL;
  } else {
    close();
  }

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT);
  servo.attach(SERVO_PIN);
  close();
}

void loop() {
  button_pressed = (digitalRead(INPUT_PIN)==HIGH);

  // Serial.println(button_pressed);

  switch (state)
  {
    case STATE_IDLE:
      if (button_pressed) {
        Serial.println("Try to open");
        open();
      }
      break;
    
    case STATE_STALL:
      if (button_pressed) {
        Serial.println("Stalled now close");
        close();
      }
      break;
  
    default:
      break;
  }
}