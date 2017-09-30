/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab1 Ex5
 */
const int DELAY_LIMIT = 4000;

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  int delay_time = 500;
  for(int i = 0; i < 10; i++)            //iterate ten times
  {
    while(delay_time <= DELAY_LIMIT)    // while dealy_time is less than 4000 add 500 ms
    {                                   // to dealy_time

      digitalWrite(LED_BUILTIN, HIGH);  //blink
      delay(delay_time);
      digitalWrite(LED_BUILTIN, LOW);
      delay(delay_time);
      delay_time += 500;                //add 500 ms
    }
    delay_time = 500;                   //reset delay_time
  }
}
