int LED = 2;
int brightness = 0;
int fadeAmount = 5;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  analogWrite(LED, brightness);

  brightness += fadeAmount;

  if(brightness <= 0 || brightness >= 255)
  {
    fadeAmount = -fadeAmount;
  }

  delay(30);

}
