/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab1 Ex4
 */

int delay_time = 1000;
// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(delay_time);                       
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(delay_time);

  //switch delay from 1000ms to 100ms
  if(delay_time == 1000)
  {
    delay_time = 100;
  }
  else if (delay_time == 100)  //switch delay from 100ms to 4000ms
  { 
    delay_time = 4000;
  }
  else delay_time = 1000;  //switch delay from 4000ms to 1000ms
}
