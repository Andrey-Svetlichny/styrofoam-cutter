/*
  PWM for нихромового резака
*/

#include "debounce.c"

const int pinButton = 9;
const int pinLed_R = 11;
const int pinLed_G = 12;
const int pinPwm = 3;
bool pwmOn = false;
const int pwmLevelDefault = 15;  // 0..255
int pwmLevel = pwmLevelDefault;
unsigned long longPressTime;
bool longPress;
struct debounce_t debounce_t = { 0, 1 };


void setup() {
  // PWM setup, see https://etechnophiles.com/change-frequency-pwm-pins-arduino-uno/

  // changing PWM frequency on D3 & D11 pins
  // TCCR2B = TCCR2B & B11111000 | B00000001; // 31372.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000010; // 3921.16 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000011; // 980.39 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000100; // 490.20 Hz (default)

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(pinLed_R, OUTPUT);
  pinMode(pinLed_G, OUTPUT);
  pinMode(pinPwm, OUTPUT);

  digitalWrite(pinLed_G, HIGH);
}


void btnPress() {
  longPressTime = millis() + 1000;
}

void btnRelease() {
  if (!longPress) {
    pwmOn = !pwmOn;

    digitalWrite(pinLed_R, pwmOn);
    digitalWrite(pinLed_G, !pwmOn);
    analogWrite(pinPwm, pwmOn ? pwmLevel : 0);
  }
  longPress = false;
}


// the loop routine runs over and over again forever:
void loop() {

  // read and debounce button
  int reading = digitalRead(pinButton);
  debounce(&debounce_t, reading, btnRelease, btnPress);

  if (debounce_t.on) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    unsigned long clk = millis();
    if (clk > longPressTime) {
      longPress = true;
      if (pwmOn) {
        longPressTime = clk + 1000;
        // increase pwmLevel, blink Green LED & beep PWM
        analogWrite(pinPwm, ++pwmLevel);
        TCCR2B = TCCR2B & B11111000 | B00000011;  // 980.39 Hz
        digitalWrite(pinLed_R, 0);
        digitalWrite(pinLed_G, 1);
        delay(300);
        TCCR2B = TCCR2B & B11111000 | B00000100;  // 490.20 Hz (default)
        digitalWrite(pinLed_R, 1);
        digitalWrite(pinLed_G, 0);
      } else {
        longPressTime = clk + 10000;
        // reset pwmLevel, blink Red/Green LED 3 times
        pwmLevel = pwmLevelDefault;
        for (int i = 0; i < 3; i++) {
          digitalWrite(pinLed_R, 1);
          digitalWrite(pinLed_G, 0);
          delay(500);
          digitalWrite(pinLed_R, 0);
          digitalWrite(pinLed_G, 1);
          delay(500);
        }
      }
    }
  }
}
