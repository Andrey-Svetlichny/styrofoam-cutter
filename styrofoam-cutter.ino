/*
  PWM for нихромового резака
*/

const int buttonPin = 9;
const int ledPin_R = 11;
const int ledPin_G = 12;
const int pwm_pin = 3;     
bool pwm_on = false;
int pwm_level = 15; // 0..255

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 100; // ms


void setup() {
  // PWM setup, see https://etechnophiles.com/change-frequency-pwm-pins-arduino-uno/

  // changing PWM frequency on D3 & D11 pins
  // TCCR2B = TCCR2B & B11111000 | B00000001; // 31372.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000010; // 3921.16 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000011; // 980.39 Hz

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin_R, OUTPUT);
  pinMode(ledPin_G, OUTPUT);
  pinMode(pwm_pin, OUTPUT);

  digitalWrite(ledPin_G, HIGH);
}

// the loop routine runs over and over again forever:
void loop() {

  // read and debounce button
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonState != reading) {
      buttonState = reading;
      if (buttonState == LOW) {
        // button press
        pwm_on = !pwm_on;
      }
    }
  }
  lastButtonState = reading;

  digitalWrite(ledPin_R, pwm_on);
  digitalWrite(ledPin_G, !pwm_on);
  analogWrite(pwm_pin, pwm_on ? pwm_level : 0);
}
