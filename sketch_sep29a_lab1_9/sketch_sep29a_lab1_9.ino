/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab1 Ex9
 */

int incomingByte = 0;   // for incoming serial data
const int LED = 2;
const int DELAY_TIME = 250;

void setup()
{
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  pinMode(LED, OUTPUT);
}

void loop() 
{

  // send data only when you receive data:
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    incomingByte = Serial.parseInt();
    
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
    
    if(incomingByte <= 0)
    {
      digitalWrite(LED, HIGH); 
    }
    else
    {
      
      for(int i = 0; i < incomingByte; i++)
      {
        digitalWrite(LED, HIGH); 
        delay(DELAY_TIME);
        digitalWrite(LED, LOW); 
        delay(DELAY_TIME);
      }
      digitalWrite(LED, HIGH);
    }
    
  }
}
