/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex6
 * Board used: Arduino Micro
 */

const int TEMP_PIN = A0;
const int MS = 1000;
int temperatureR;


void setup() 
{
  Serial.begin(9600);
  pinMode(TEMP_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
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
  delay(MS);
  
  // temperature range  [-40, 215] deg F
  analogWrite(LED_BUILTIN, temperatureF + 40);

//  if(temperatureF >= 77.0)
//  {
//    analogWrite(LED_BUILTIN, 255);
//
//  }
//  else
//  {
//    analogWrite(LED_BUILTIN, temperatureF - 40);
//  }
  
}
