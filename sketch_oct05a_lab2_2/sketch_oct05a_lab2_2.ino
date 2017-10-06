/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex2
 * Board used: Arduino Micro
 */
 
const int LED_PIN = 2; //D2
const int PHOTOR_PIN = 4; //D4
int ledState;

void setup() 
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(PHOTOR_PIN, INPUT);
    ledState = 0;
}

void loop()
{
   ledState = digitalRead(PHOTOR_PIN);
   Serial.println("ledState = " + String(ledState));

   if(ledState == 1)  //1 is ON
   {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Status: LED ON");
   }
   else   //0 is OFF
   {
      digitalWrite(LED_PIN, LOW);
      Serial.println("Status: LED OFF");
   }

   //slow down the transmission for effective Serial communication.
   delay(50);
}
