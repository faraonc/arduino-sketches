/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex4
 * Board used: Arduino Micro
 */

const int TEMP_PIN = A0;
int temperatureR;
int incomingByte;
int state;

void setup() 
{
  Serial.begin(9600);
  pinMode(TEMP_PIN, INPUT);
  temperatureR = 0;
  incomingByte = 0;
  state = 2;

}

void loop() 
{
  if(Serial.available() > 0)
  {
    // read the incoming byte:
      incomingByte = Serial.parseInt();

      //if key pressed is '1', turn off
      if(incomingByte == 1 && state != 1)
      {
         Serial.println("Disabled\n");
         state = 1;
      }
      else if(incomingByte == 2 && state != 2) //else turn on
      {
        Serial.println("Enabled\n");
        state = 2;
      }
    
  }

  if(state == 2)
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
    Serial.println(" deg F\n");
    delay(1000);
  }
  
 
}
