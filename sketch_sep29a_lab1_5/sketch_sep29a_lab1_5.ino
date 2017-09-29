
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  int delay_time = 500;
  const int DELAY_LIMIT = 4000;
  for(int i = 0; i < 10; i++)
  {
    while(delay_time <= DELAY_LIMIT)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(delay_time);
      digitalWrite(LED_BUILTIN, LOW);
      delay(delay_time);
      delay_time += 500;
    }
    delay_time = 500;
  }
}
