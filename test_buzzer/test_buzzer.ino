const int BUZZER = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER, OUTPUT);

}

void loop() {
  tone(BUZZER, 1000);
  delay(1000);
  noTone(BUZZER);
  delay(1000);
}
