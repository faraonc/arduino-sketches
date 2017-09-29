/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab1 Ex10
 */

// for incoming serial data
int brightness = 0;
int fadeAmount = 5;
int incomingByte = 0;
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
      
    if(incomingByte > 0 && incomingByte <= 255) //check if the input inside the range
    {
      fadeAmount = incomingByte; 
      brightness = 0;                    //reset the brightness 
    }
    else if (incomingByte == 0)
    {
      fadeAmount = 0; //fadeAmount = incomingByte; in order to keep the current brightness
    }
   }
    //fading part                                 
    analogWrite(LED, brightness);

    brightness += fadeAmount;

    if(brightness <= 0 || brightness >= 255)
    {
      fadeAmount = -fadeAmount;
    }

    delay(30);
}
