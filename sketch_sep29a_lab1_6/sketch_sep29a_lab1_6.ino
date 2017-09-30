/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab1 Ex6
 */
const int DELAY_LIMIT = 4000;
// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600); 
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  int delay_time = 500;
  delay(1000);  //just to be able to get all the stdout including the initial iteration
  
  for(int i = 0; i < 10; i++)
  {
    while(delay_time <= DELAY_LIMIT)
    {
      //print iteration # and delay time
      Serial.println("Iteration # " + String(i));        
      Serial.println("Blinking HIGH for " + String(delay_time));
      
      //blink
      digitalWrite(LED_BUILTIN, HIGH);
      delay(delay_time);
      
      
      Serial.println("Iteration # " + String(i));               //print
      Serial.println("Blinking LOW for " + String(delay_time));
      
      digitalWrite(LED_BUILTIN, LOW);
      delay(delay_time);
      delay_time += 500;
      
    }
    delay_time = 500;             //reset delay timer
  }
}
