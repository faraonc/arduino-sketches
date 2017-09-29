/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab1 Ex3
 */
 
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, true);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, false);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
