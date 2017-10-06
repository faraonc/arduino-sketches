/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex1
 * Board used: Arduino Micro
 */

const int PHOTOR_PIN = A2;
int lightLevel;

void setup() 
{
    Serial.begin(9600);
    pinMode(PHOTOR_PIN, INPUT);
    lightLevel = 0;
}

void loop()
{
   lightLevel = analogRead(PHOTOR_PIN);
   Serial.println(lightLevel);

   //slow down the transmission for effective Serial communication.
   delay(50);
}
