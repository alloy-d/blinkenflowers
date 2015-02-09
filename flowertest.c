int pin_r = 9;
int pin_g = 10;
int pin_b = 11;

int brightness = 0;
int increment = 1;

void setup() {
  pinMode(pin_r, OUTPUT);
  pinMode(pin_g, OUTPUT);
  pinMode(pin_b, OUTPUT);
  /* Serial.begin(9600); */
}

void loop() {
  brightness = brightness + increment;

  if (brightness <= 0 || brightness >= 128) {
    increment = -increment;
  }

  analogWrite(pin_r, brightness/2);
  analogWrite(pin_g, brightness);
  analogWrite(pin_b, 128);

  delay(20);
}
