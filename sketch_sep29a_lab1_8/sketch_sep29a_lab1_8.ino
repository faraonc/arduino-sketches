/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab1 Ex8
 */
int stateTracker = 0;
int incomingByte = 0;   // for incoming serial data
const int LED = 2;

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

      //if key pressed is '1', turn off
      if(incomingByte == 1)
      {
        digitalWrite(LED, LOW);
        stateTracker = 0; 
      }
      else if(incomingByte == 2)  //if key pressed is '2', turn on
      {
        digitalWrite(LED, HIGH);
        stateTracker = 1;
      }
      else if(incomingByte == 3)  //if key pressed is '3', toggle
      {
        if (stateTracker == 0)
        {
          digitalWrite(LED, HIGH);
          stateTracker = 1;
        }
        else 
        {
          digitalWrite(LED, LOW);
          stateTracker = 0;
        }
      }
  }
}
