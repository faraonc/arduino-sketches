/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex5
 * Board used: Arduino Micro
 */

const int TEMP_PIN = A0;
const int MS = 1000;
int temperatureR;
int incomingByte;
int stateFreq;

void setup() 
{
  Serial.begin(9600);
  pinMode(TEMP_PIN, INPUT);
  temperatureR = 0;
  incomingByte = 0;
  stateFreq = 0;
}

void loop() 
{
  if(Serial.available() > 0)
  {
    // read the incoming byte:
      incomingByte = Serial.parseInt();

      //if value is '0', turn off
      if(incomingByte <= 0 && stateFreq != 0)
      {
         Serial.println("\nDisabled\n");
         stateFreq = 0;
      }
      else //else turn on
      {
        Serial.println("Enabled\n");
        stateFreq = MS / incomingByte;
        Serial.print("Adjust sampling rate ");
        Serial.print(incomingByte);
        Serial.println(" hz");
      }
    
  }

  if(stateFreq > 0)
  {
    temperatureR = analogRead(TEMP_PIN);

    float voltage = temperatureR * 5.0;
    voltage /= 1024.0;
    Serial.println();
    Serial.print(voltage); 
    Serial.println(" volts");
  
    float temperatureC = (voltage - 0.5) * 100;
    Serial.print(temperatureC);
    Serial.println(" deg C"); 
  
    float temperatureF = (temperatureC * 9.0 / 5.) + 32.0;
    Serial.print(temperatureF);
    Serial.println(" deg F");

    Serial.print("Sampling rate ");
    Serial.print(incomingByte);
    Serial.println(" hz");
        
    delay(stateFreq);
  }
  
 
}
