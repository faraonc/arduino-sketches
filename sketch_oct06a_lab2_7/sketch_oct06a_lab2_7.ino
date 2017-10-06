/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex7
 * Board used: Arduino Micro
 */

const int TEMP_PIN = A0;
const int LED_PIN = 2;
const int BUZZER = 9;
const int MS = 1000;
int temperatureR;
int incomingByte; //threshold


void setup() 
{
  Serial.begin(9600);
  pinMode(TEMP_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  incomingByte = 100; //starting threshold
}

void loop() 
{
  if(Serial.available() > 0)
  {
    // read the incoming byte:
    incomingByte = Serial.parseInt();
    Serial.print("Threshold is set to ");
    Serial.print(incomingByte);
    Serial.println(" deg F.");
  }
  
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
  delay(MS);

  Serial.print(incomingByte);
  Serial.println(" deg F threshold\n");
  
  if (temperatureF > incomingByte)
  {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER, MS);

  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER);
  }

}
