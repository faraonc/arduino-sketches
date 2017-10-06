/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex3
 * Board used: Arduino Micro
 */

const int TEMP_PIN = A0;
int temperatureR;

void setup() 
{
  Serial.begin(9600);
  pinMode(TEMP_PIN, INPUT);
  temperatureR = 0;

}

void loop() 
{
  temperatureR = analogRead(TEMP_PIN);
  
  float voltage = temperatureR * 5.0;
  voltage /= 1024.0;
  Serial.print(voltage); 
  Serial.println(" volts");

  float temperatureC = (voltage - 0.5) * 100;
  Serial.print(temperatureC);
  Serial.println(" deg C"); 

  float temperatureF = (temperatureC * 9.0 / 5.) + 32.0;
  Serial.print(temperatureF);
  Serial.println(" deg F");
  
  delay(1000);
}
